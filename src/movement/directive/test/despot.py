# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import math
import os
import sys
import time
from copy import deepcopy
from random import random
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator

def main(args):
    com = Communicator(
            module_name=args.module_name,
            settings_path=args.settings_path)

    # These values represent the submarine's membership in 8 fuzzy sets.
    # These sets come in pairs (left/right, back/forward, etc.) and represent
    # where the submarine is in relation to where it wants to be.
    # The roll and yaw sets are not available to directive module since
    # they should be controlled by either the stabilization module, or by
    # the awesome balancing skills of the mech-e's.
    packet = {
            'is_left': 0.0,
            'is_right': 0.0,
            'is_back': 0.0,
            'is_forward': 0.0,
            'is_low': 0.0,
            'is_high': 0.0,
            'is_rotated_left': 0.0,
            'is_rotated_right': 0.0}

    missive = {
            "desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
            "desired_orientation": {"x": 0.0, "y": 0.0, "z": 0.0},
            "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0},
            "timestamp": time.time()}

    last_timestamp = 0.0
    while True:
        missive = com.get_last_message("decision")

        if missive and missive['timestamp'] > last_timestamp:
            last_timestamp = missive['timestamp']
            tx_packet = deepcopy(packet)

            if (missive['desired_offset']['y'] == 9001.0 and
                missive['desired_velocity']['y'] == 1.0):
                tx_packet['is_back'] = 1.0
            elif (missive['desired_offset']['y'] == -9001 and
                  missive['desired_velocity']['y'] == -1.0):
                tx_packet['is_forward'] = 1.0
            elif missive['desired_orientation']['z'] == 3 * math.pi / 2:
                tx_packet['is_rotated_right'] = 1.0
            elif missive['desired_orientation']['z'] == math.pi / 2:
                tx_packet['is_rotated_left'] = 1.0
            elif (missive['desired_offset']['z'] == 9001 and
                  missive['desired_velocity']['z'] == 1.0):
                tx_packet['is_low'] = 1.0
            elif (missive['desired_offset']['z'] == -9001 and
                  missive['desired_velocity']['z'] == -1.0):
                tx_packet['is_high'] = 1.0

            print tx_packet
            com.publish_message(tx_packet)
        time.sleep(args.epoch)


def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('--settings_path', type=str,
            default=None,
            help='Settings file path.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/directive',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

