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

DEBUG = True

def cmd_thruster(thruster_id, magnitude, direction):
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
    if DEBUG:
        print 'raw_cmd:', [ord(x) for x in raw_cmd]
        return
    else:
        cmd_thruster.ser.write(raw_cmd)

def main(args):
    if not DEBUG:
        ser = serial.Serial()
        # this may change, depending on what port the OS gives the microcontroller
        ser.port = args.port
        # the baudrate may change in the future
        ser.baudrate = args.baudrate

        # attempt to open the serial port (there is no guard code, I'm assuming
        # this does not fail)
        ser.open()
        cmd_thruster.ser = ser

    com = Communicator(module_name=args.module_name,
                       settings_path=args.settings_path)
    mag = args.magnitude

    while True:
        stabalization_packet = com.get_last_message("movement/stabalization")
        if stabalization_packet:
            if stabalization_packet['vector']['x'] == 1.0:
                pass
            elif stabalization_packet['vector']['x'] == -1.0:
                pass
            elif stabalization_packet['vector']['y'] == 1.0:
                """causes the sub to move forward"""
                cmd_thruster(THRUSTER_BOW_SB, mag, 0)
                cmd_thruster(THRUSTER_BOW_PORT, mag, 0)
                cmd_thruster(THRUSTER_STERN_SB, mag, 0)
                cmd_thruster(THRUSTER_STERN_PORT, mag, 0)
            elif stabalization_packet['vector']['y'] == -1.0:
                """causes the sub to move backwards"""
                cmd_thruster(THRUSTER_BOW_SB, mag, 1)
                cmd_thruster(THRUSTER_BOW_PORT, mag, 1)
                cmd_thruster(THRUSTER_STERN_SB, mag, 1)
                cmd_thruster(THRUSTER_STERN_PORT, mag, 1)
            elif stabalization_packet['vector']['z'] == 1.0:
                """causes the sub to surface"""
                cmd_thruster(THRUSTER_DEPTH_SB, mag, 1)
                cmd_thruster(THRUSTER_DEPTH_PORT, mag, 1)
            elif stabalization_packet['vector']['z'] == -1.0:
                """causes the sub to dive"""
                cmd_thruster(THRUSTER_DEPTH_SB, mag, 0)
                cmd_thruster(THRUSTER_DEPTH_PORT, mag, 0)
            elif stabalization_packet['rotation']['z'] == 1.0:
                """causes the sub to rotate clockwise"""
                cmd_thruster(THRUSTER_BOW_SB, mag, 0)
                cmd_thruster(THRUSTER_BOW_PORT, mag, 1)
                cmd_thruster(THRUSTER_STERN_SB, mag, 1)
                cmd_thruster(THRUSTER_STERN_PORT, mag, 0)
            elif stabalization_packet['rotation']['z'] == -1.0:
                """causes the sub to rotate counter-clockwise"""
                cmd_thruster(THRUSTER_BOW_SB, mag, 1)
                cmd_thruster(THRUSTER_BOW_PORT, mag, 0)
                cmd_thruster(THRUSTER_STERN_SB, mag, 0)
                cmd_thruster(THRUSTER_STERN_PORT, mag, 1)
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
            default=25,
            help='Thruster magnitude in percent.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

