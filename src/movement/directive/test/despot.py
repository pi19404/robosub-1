# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
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
    # they should be controlled by either the stabalization module, or by
    # the awesome balancing skills of the mech-e's.
    packet = {
            'is_left': 0.0,
            'is_right': 0.0,
            'is_back': 1.0,
            'is_forward': 0.0,
            'is_low': 0.0,
            'is_high': 0.0,
            'is_rotated_left': 0.0,
            'is_rotated_right': 0.0}

    while True:
        com.publish_message(packet)
        time.sleep(args.epoch)
        for mname in com.listening():
            recv_msg = com.get_last_message(mname)
            if recv_msg:
                pass
                #print recv_msg

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

