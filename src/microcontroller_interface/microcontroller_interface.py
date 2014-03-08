# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
import serial
sys.path.append(os.path.abspath("../"))
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

def cmd_thruster(thruster_id, value):
    """
    cmd_thruster() sends a thruster control command to the microncontroller
        It takes an id, and a value between +127 and -127 (negative is reverse)
        *Note that two of the thrusters are wired backwards, so the sign of the input
        should be flipped

    -------------------------
    |                       |
---------               ---------
|BOW    |               |BOW *  |
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
|STERN* |               |STERN  |
|PORT   |               |STARBOA|
---------               ---------
    |                       |
    -------------------------

    """

    raw_thruster_id = '\0'
    raw_value = '\0'
    raw_cmd = ""

    # the chr() command converts the integer to the ascii character
    # representation, which is a raw byte

    # convert the thruster id to a raw binary value
    raw_thruster_id = chr(thruster_id)

    #make sure value is within bounds
    if (value > 127) :
        value = 127
    elif (value < -127) :
        value = -127

    #convert value variable into a raw byte
    raw_value = chr(value & 0xFF)

    # combine the raw bytes
    raw_cmd = CONTROL_BYTE + raw_thruster_id + raw_value

    # send the commmand to the microcontroller
    if not DEBUG:
        cmd_thruster.ser.write(raw_cmd)
    return raw_cmd

def get_lock(ser, com):
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

def get_packet(ser, com):
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
            com.debug("Error: lost sync.")
            #raw_input() # waits for the user to press the enter key
            ser.flushInput() # flushes the serial rx buffer
            get_lock(ser, com) # get back into sync
        else:
            # if we are in sync, break out of loop
            success = True

        return packet

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

    com = Communicator("microcontroller")
    accel_com = Communicator('datafeed/raw/accelerometer')
    gyro_com = Communicator('datafeed/raw/gyroscope')
    compass_com = Communicator('datafeed/raw/compass')
    depth_com = Communicator('datafeed/raw/depth')
    battery_voltage_com = Communicator('datafeed/raw/battery_voltage')

    # If we want to insert a mock module for these sensors, we want to prevent
    # multiple input sources from inserting messages into the pipe.
    disabled_publish = lambda *args: None
    if args.disable_accel_com:
        accel_com.publish_message = disabled_publish
    if args.disable_gyro_com:
        gyro_com.publish_message = disabled_publish
    if args.disable_compass_com:
        compass_com.publish_message = disabled_publish
    if args.disable_depth_com:
        depth_com.publish_message = disabled_publish
    if args.disable_battery_voltage_com:
        battery_voltage_com.publish_message = disabled_publish


    if not DEBUG:
        ser = serial.Serial()
        cmd_thruster.ser = ser
        # this may change, depending on what port the OS gives the
        # microcontroller
        ser.port = args.port
        # the baudrate may change in the future
        ser.baudrate = args.baudrate

        # attempt to open the serial port (there is no guard code, I'm assuming
        # this does not fail)
        ser.open()
        get_lock(ser, com) # get in sync with the stream

    prev_gv_timestamp = 0.0
    while True:
        gv_packet = com.get_last_message("movement/translation")
        # handle values from grapevine.py
        if gv_packet['timestamp'] > prev_gv_timestamp:
            cmd_thruster(THRUSTER_BOW_PORT, gv_packet["Thruster_Values"]["front_left"])
            cmd_thruster(THRUSTER_BOW_SB, gv_packet["Thruster_Values"]["front_right"])
            cmd_thruster(THRUSTER_DEPTH_PORT, gv_packet["Thruster_Values"]["middle_left"])
            cmd_thruster(THRUSTER_DEPTH_SB, gv_packet["Thruster_Values"]["middle_right"])
            cmd_thruster(THRUSTER_STERN_PORT, gv_packet["Thruster_Values"]["back_left"])
            cmd_thruster(THRUSTER_STERN_SB, gv_packet["Thruster_Values"]["back_right"])
            prev_gv_timestamp = gv_packet['timestamp']
        # handle values from uC, USB port.
        if not DEBUG:
            uC_packet = get_packet(ser, com)
            respond_to_serial_packet(uC_packet, accel_com, gyro_com, compass_com,
                                    depth_com, battery_voltage_com)

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
    parser.add_argument(
            "--disable_accel_com", default=False, action="store_true")
    parser.add_argument(
            "--disable_gyro_com", default=False, action="store_true")
    parser.add_argument(
            "--disable_compass_com", default=False, action="store_true")
    parser.add_argument(
            "--disable_depth_com", default=False, action="store_true")
    parser.add_argument(
            "--disable_battery_voltage_com", default=False,
            action="store_true")
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
