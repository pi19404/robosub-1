# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
import serial
from random import random
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator

CONTROL_BYTE = '\n'
THRUSTER_BOW_SB = 0x10
THRUSTER_BOW_PORT = 0x11
THRUSTER_DEPTH_SB = 0x12
THRUSTER_DEPTH_PORT = 0x13
THRUSTER_STERN_SB = 0x14
THRUSTER_STERN_PORT = 0x15

DEBUG = None

def cmd_thruster(thruster_id, magnitude, direction, debug=False):
    """
    cmd_thruster() sends a thruster control command to the microncontroller
    It takes an id, a magnitude (between 0 and 100), and a direction (0 or 1)
    0 is forward, 1 is reverse

    """

    raw_thruster_id = '\0'
    raw_direction_mag = '\0'
    raw_cmd = ""

    # the chr() command converts the integer to the ascii character
    # representation, which is a raw byte

    # convert the thruster id to a raw binary value
    raw_thruster_id = chr(thruster_id)

    # make sure magnitude is within bounds
    if magnitude > 100:
        magnitude = 100
    elif magnitude < 0:
        magnitude = 0

    # convert the magnitude from a percentage value to a value between 0 and
    # 127
    magnitude = magnitude * 127 // 100

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


def main(args):
    # Someone SHOULD complain about this.
    global DEBUG
    DEBUG = args.debug

    if not DEBUG:
        ser = serial.Serial()
        # this may change, depending on what port the OS gives the microcontroller
        ser.port = "/dev/ttyUSB0" #args.port
        # the baudrate may change in the future
        ser.baudrate = args.baudrate

        # attempt to open the serial port (there is no guard code, I'm assuming
        # this does not fail)
        ser.open()
        cmd_thruster.ser = ser

    com = Communicator(module_name=args.module_name,
                       settings_path=args.settings_path)

    mag = args.magnitude
    last_packet_time = 0.0
    while True:
        stabalization_packet = com.get_last_message("movement/stabalization")
        if (stabalization_packet and
            stabalization_packet['timestamp'] > last_packet_time):
            # TODO: This would allow us to use cleaner debug messages if we
            # instead had a thruster settings dictionary. E.g.:
            # {'port': {'bow': (0, 0), 'port': (0, 0), 'stern': (0, 0)},
            #  'starboard': {'bow': (0, 0), 'port': (0, 0), 'stern': (0, 0)}}
            raw_cmds = []
            if stabalization_packet['vector']['x'] == 1.0:
                intent = 'strafe left (not implemented)'
            elif stabalization_packet['vector']['x'] == -1.0:
                intent = 'strafe right (not implemented)'
            elif stabalization_packet['vector']['y'] == 1.0:
                # causes the sub to move forward
                intent = 'move forward'
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 1))
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 1))
            elif stabalization_packet['vector']['y'] == -1.0:
                # causes the sub to move backwards
                intent = 'move backward'
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 1))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 1))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 0))
            elif stabalization_packet['vector']['z'] == 1.0:
                # causes the sub to surface
                intent = 'rise'
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, mag, 1))
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, mag, 0))
            elif stabalization_packet['vector']['z'] == -1.0:
                # causes the sub to dive
                intent = 'dive'
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 100, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 100, 0))
            elif stabalization_packet['rotation']['z'] == 1.0:
                # causes the sub to rotate clockwise
                intent = 'rotate right'
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 0))
                #raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 0))
                #raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 0))
            elif stabalization_packet['rotation']['z'] == -1.0:
                # causes the sub to rotate counter-clockwise
                intent = 'rotate left'
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, mag, 1))
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, mag, 1))
                #raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, mag, 1))
                #raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, mag, 1))
            else:
                # Turn off all thrusters.
                intent = 'full stop'
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_SB, 0, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_BOW_PORT, 0, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_SB, 0, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_DEPTH_PORT, 0, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_SB, 0, 0))
                raw_cmds.append(cmd_thruster(THRUSTER_STERN_PORT, 0, 1))

            msg = {"intent": intent,
                   "raw_cmds": [[ord(x) for x in cmd] for cmd in raw_cmds]}
            print msg
            com.publish_message(msg)
            time.sleep(args.epoch)
    ser.close()

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('--settings_path', type=str,
            default=None,
            help='Settings file path.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/physical',
            help='Module name.')
    parser.add_argument('-b', '--baudrate', type=int,
            default=56818,
            help="Serial interface baudrate.")
    parser.add_argument('-p', '--port', type=str,
            default='/dev/ttyUSB0',
            help="Serial interface port.")
    parser.add_argument('--magnitude', type=str,
            default=100,
            help='Thruster magnitude in percent.')
    parser.add_argument('-d', '--debug',
            default=False,
            action="store_true",
            help='Set debug mode to True.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

