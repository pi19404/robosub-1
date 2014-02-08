# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
import serial
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator

CONTROL_BYTE = '\n'
ACL_1_X_addr = 0x10
ACL_1_Y_addr = 0x11
ACL_1_Z_addr = 0x12
GYRO_1_X_addr = 0x20
GYRO_1_Y_addr = 0x21
GYRO_1_Z_addr = 0x22

ADC_DEPTH = 0x30
ADC_BATT = 0x31

THRUSTER_BOW_SB = 0x10
THRUSTER_BOW_PORT = 0x11
THRUSTER_DEPTH_SB = 0x12
THRUSTER_DEPTH_PORT = 0x13
THRUSTER_STERN_SB = 0x14
THRUSTER_STERN_PORT = 0x15

DEBUG = None

#Initialize some variables
ACL_1_X_val = -1
ACL_1_Y_val = -1
ACL_1_Z_val = -1
GYRO_1_X_val = -1
GYRO_1_Y_val = -1
GYRO_1_Z_val = -1
ADC_DEPTH_val = -1
ADC_BATT_val = -1

def cmd_thruster(thruster_id, magnitude, direction):
    """
    cmd_thruster() sends a thruster control command to the microncontroller It
    takes an id, a magnitude (between 0 and 100), and a direction (0 or 1) 0 is
    forward, 1 is reverse

    -------------------------
    |                       |
---------               ---------
|BOW    |               |BOW    |
|PORT   |               |STARBOA|
---------               ---------
    |                       |
    |                       |
    |                       |
---------               ---------
|DEPTH  |               |DEPTH  |
|PORT   |               |STARBOA|
---------               ---------
    |                       |
    |                       |
    |                       |
---------               ---------
|STERN  |               |STERN  |
|PORT   |               |STARBOA|
---------               ---------
    |                       |
    -------------------------

    """

    raw_thruster_id = '\0'
    raw_direction_mag = '\0'
    raw_cmd = ""

    # the chr() command converts the integer to the ascii character
    # representation, which is a raw byte

    # convert the thruster id to a raw binary value
    raw_thruster_id = chr(thruster_id)

    # make sure magnitude is within bounds
   #if magnitude > 100:
   #    magnitude = 100
   #elif magnitude < 0:
   #    magnitude = 0

    # convert the magnitude from a percentage value to a value between 0 and
    # 127
    #magnitude = magnitude * 127 // 100

    # make sure direction is only one bit
    direction &= 0x01

    # combine magnitude and direction data into one variable
    raw_direction_mag = chr((direction << 7) | magnitude)

    # combine the raw bytes
    raw_cmd = CONTROL_BYTE + raw_thruster_id + raw_direction_mag

    # send the commmand to the microcontroller
    if not DEBUG:
        cmd_thruster.ser.write(raw_cmd)
    return raw_cmd

def get_lock(ser):
    """
    Here we are trying to make sure we have actually found a control byte, so
    we receive several packets, then look at where we expect the control bytes
    to be. If they are not in the expected locations, we wait for a new control
    byte and try again.

    X000X000X
    012345678

    """
    # variables for the sync loop
    current_byte = '\0'
    packet_array = ""
    in_sync = False

    # reset the serial port
    ser.close()
    ser.open()

    print "Aquiring stream sync"

    while not in_sync:
        # read a packet from the serial port
        current_byte = ser.read()

        # if the byte is the CONTROL_BYTE, then receive several packets
        # otherwise, we will jump back to the top of the loop and get
        # another byte
        if current_byte == CONTROL_BYTE:
            packet_array = "" # clear out the array
            packet_array += current_byte # add the byte to the array

            # receive several packets
            while len(packet_array) != 9:
                packet_array += ser.read()

            # check to see if the control byte is in the proper
            # location in the received packets
            if (packet_array[0] == CONTROL_BYTE and
                packet_array[4] == CONTROL_BYTE and
                packet_array[8] == CONTROL_BYTE):

                # throw away rest of last packet
                ser.read(3)

                # say we are in sync so we can break out of the
                # loop
                in_sync = True
                print "sync locked"
# end get_lock()

def get_packet(ser):
    """
    This function reads a 4-byte packet from the serial port.  It will also
    check to make sure we are still in sync, and pauses the program if we lose
    sync. It will then attempt to get back into sync with the serial stream.

    """
    success = False

    while not success:
        # read 4 bytes from the serial port
        packet = ser.read(4)
        # ensure we are in sync by checking that the control byte is in the
        # correct place
        if packet[0] != CONTROL_BYTE: # if we are not in sync
            print "Error: lost sync. Press the [Enter] key to attempt to re-sync"
            #raw_input() # waits for the user to press the enter key
            ser.flushInput() # flushes the serial rx buffer
            get_lock(ser) # get back into sync
        else:
            # if we are in sync, break out of loop
            success = True

        return packet

def respond_to_stabilization_packet(packet, mag, advisor_packet=None):
    # TODO: This would allow us to use cleaner debug messages if we
    # instead had a thruster settings dictionary. E.g.:
    # {'port': {'bow': (0, 0), 'port': (0, 0), 'stern': (0, 0)},
    # 'starboard': {'bow': (0, 0), 'port': (0, 0), 'stern': (0, 0)}}
    raw_cmds = []
    intent = None
    # Node: the advisor_packet is for debugging purposes only.
    if advisor_packet and advisor_packet['command'] == 'stop':
        # Turn off all thrusters.
        intent = 'full stop'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 0))
    elif advisor_packet and advisor_packet['command'] == 'roll left':
        intent = 'roll left'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 1))
    elif advisor_packet and advisor_packet['command'] == 'roll right':
        intent = 'roll right'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 1))
    elif packet['vector']['x'] > 0.0:
        intent = 'strafe left (not implemented)'
    elif packet['vector']['x'] < 0.0:
        intent = 'strafe right (not implemented)'
    elif packet['vector']['y'] > 0.0:
        # causes the sub to move forward
        intent = 'move forward'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 1))
    elif packet['vector']['y'] < 0.0:
        # causes the sub to move backwards
        intent = 'move backward'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 0))
    elif packet['vector']['z'] > 0.0:
        # causes the sub to surface
        intent = 'rise'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 1))
    elif packet['vector']['z'] < -0.0:
        # causes the sub to dive
        intent = 'dive'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 0))
    elif packet['rotation']['yaw'] > 0.0:
        # causes the sub to rotate clockwise
        intent = 'rotate right'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 1))
    elif packet['rotation']['yaw'] < -0.0:
        # causes the sub to rotate counter-clockwise
        intent = 'rotate left'
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 1))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 0))
        raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 0))

    return intent, raw_cmds

def respond_to_serial_packet(packet, accel_com, gyro_com, compass_com,
                             depth_com, battery_voltage_com):
    # pull the device information out of the packet second byte of packet
    # is device information (first byte is always control byte)
    device = ord(packet[1])

    # if-elif statement looks for what device the packet is concerning, and
    # handles the data appropriately
    if device == ACL_1_X_addr:
        # pulls the data out the last two bytes of the packet
        ACL_1_X_val = ord(packet[2]) | (ord(packet[3]) << 8)

        # data is stored in 2's complement form, this does the appropriate
        # conversion
        if ACL_1_X_val > 32767:
            ACL_1_X_val = (ACL_1_X_val-65536)

        accel_com.publish_message({"ACL_X": ACL_1_X_val})


    elif device == ACL_1_Y_addr:
        ACL_1_Y_val = ord(packet[2]) | (ord(packet[3]) << 8)

        if ACL_1_Y_val > 32767:
            ACL_1_Y_val = (ACL_1_Y_val-65536)

        accel_com.publish_message({"ACL_Y": ACL_1_Y_val})

    elif device == ACL_1_Z_addr:
        ACL_1_Z_val = ord(packet[2]) | (ord(packet[3]) << 8)

        if ACL_1_Z_val > 32767:
            ACL_1_Z_val = (ACL_1_Z_val-65536)

        accel_com.publish_message({"ACL_Z": ACL_1_Z_val})

    elif device == GYRO_1_X_addr:
        GYRO_1_X_val = ord(packet[2]) | (ord(packet[3]) << 8)

        if GYRO_1_X_val > 32767:
            GYRO_1_X_val = (GYRO_1_X_val-65536)

        # XXX com?
        gyro_com.publish_message({"GYRO_X": GYRO_1_X_val})
    elif device == GYRO_1_Y_addr:
        GYRO_1_Y_val = ord(packet[2]) | (ord(packet[3]) << 8)

        if GYRO_1_Y_val > 32767:
            GYRO_1_Y_val = (GYRO_1_Y_val-65536)

        # XXX com?
        gyro_com.publish_message({"GYRO_Y": GYRO_1_Y_val})
    elif device == GYRO_1_Z_addr:
        GYRO_1_Z_val = ord(packet[2]) | (ord(packet[3]) << 8)
        if GYRO_1_Z_val > 32767:
            GYRO_1_Z_val = (GYRO_1_Z_val-65536)

        # XXX com?
        gyro_com.publish_message({"GYRO_Z": GYRO_1_Z_val})
    elif device == ADC_DEPTH:
        ADC_DEPTH_val = ord(packet[2]) | (ord(packet[3]) << 8)

        # XXX Shouldn't this be depth_com?
        depth_com.publish_message({"DEPTH": ADC_DEPTH_val})
    elif device == ADC_BATT:
        ADC_BATT_val = ord(packet[2]) | (ord(packet[3]) << 8)
        ADC_BATT_val = ((ADC_BATT_val) * 3.3/1024 * 7.5)

        # XXX com?
        battery_voltage_com.publish_message({"BATTERY_VOLTAGE": ADC_BATT_val})


def main(args):
    # Someone SHOULD complain about this.
    global DEBUG
    DEBUG = args.debug

    # Expected: args.module_name == "movement/physical"
    com = Communicator(module_name=args.module_name)
    accel_com = Communicator(module_name='sensor/accelerometer')
    gyro_com = Communicator(module_name='sensor/gyroscope')
    compass_com = Communicator(module_name='sensor/compass')
    depth_com = Communicator(module_name='sensor/depth')
    battery_voltage_com = Communicator(module_name='sensor/battery_voltage')

    if not DEBUG:
        ser = serial.Serial()
        # this may change, depending on what port the OS gives the
        # microcontroller
        ser.port = args.port
        # the baudrate may change in the future
        ser.baudrate = args.baudrate

        # attempt to open the serial port (there is no guard code, I'm assuming
        # this does not fail)
        ser.open()
        cmd_thruster.ser = ser

        get_lock(ser) # get in sync with the stream

    mag = int(args.magnitude)
    last_packet_time = 0.0
    last_advisor_packet = None
    last_advisor_packet_time = 0.0
    while True:
        stabilization_packet = com.get_last_message("movement/stabilization")
        advisor_packet = com.get_last_message("decision/advisor")
        new_event = False

        if (advisor_packet and
            advisor_packet['timestamp'] > last_advisor_packet_time):
            last_advisor_packet_time = advisor_packet['timestamp']
            last_advisor_packet = advisor_packet
            mag = int(advisor_packet['magnitude'])
            if advisor_packet['command'] == 'stop':
                new_event = True

        if (stabilization_packet and
            stabilization_packet['timestamp'] > last_packet_time):
            last_packet_time = stabilization_packet['timestamp']
            new_event = True

        if new_event:
            intent, raw_cmds = respond_to_stabilization_packet(
                    packet=stabilization_packet, mag=mag,
                    advisor_packet=last_advisor_packet)
            # Debugging info...
            msg = {"intent": intent,
                   "raw_cmds": [[ord(x) for x in cmd] for cmd in raw_cmds]}
            print msg
            com.publish_message(msg)

        # receive a packet
        if not DEBUG:
            received_packet = get_packet(ser)
            respond_to_serial_packet(
                    received_packet, accel_com, gyro_com, compass_com, depth_com,
                    battery_voltage_com)

        time.sleep(args.epoch)

    if not DEBUG:
        ser.close()

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument(
            '-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument(
            '-m', '--module_name', type=str,
            default='movement/physical',
            help='Module name.')
    parser.add_argument(
            '-b', '--baudrate', type=int,
            default=56818,
            help="Serial interface baudrate.")
    parser.add_argument(
            '-p', '--port', type=str,
            default='/dev/ttyUSB0',
            help="Serial interface port.")
    parser.add_argument(
            '--magnitude', type=int,
            default="100",
            help='Thruster magnitude in percent.')
    parser.add_argument(
            '-d', '--debug',
            default=False,
            action="store_true",
            help='Set debug mode to True.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
