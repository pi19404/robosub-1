# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
from copy import deepcopy
from random import random
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator

def main(args):
    com = Communicator(
            module_name=args.module_name,
            settings_path=args.settings_path)

    # TODO(LPE) We need to settle on a good convention for these vectors.
    # For now, I'm using these conventions:
    # vector:
    #   x is left and right
    #   y is forward and backward
    #   z is up and down.
    # rotation:
    #   x is pitch
    #   y is roll
    #   z is yaw. Negative yaws left, positive yaws right.
    packet = {
            'vector': {'x': 0.0, 'y': 0.0, 'z': 0.0},
            'rotation': {'x': 0.0, 'y': 0.0, 'z': 0.0}}

    while True:
        directive_packet = com.get_last_message("movement/directive")
        if directive_packet:
            # Kludges to handle keyboard inputs.
            tx_packet = deepcopy(packet)
            if directive_packet['is_left'] == 1.0:
                tx_packet['vector']['x'] = 1.0
            elif directive_packet['is_right'] == 1.0:
                tx_packet['vector']['x'] = -1.0
            elif directive_packet['is_back'] == 1.0:
                tx_packet['vector']['y'] = 1.0
            elif directive_packet['is_forward'] == 1.0:
                tx_packet['vector']['y'] = -1.0
            elif directive_packet['is_low'] == 1.0:
                tx_packet['vector']['z'] = 1.0
            elif directive_packet['is_high'] == 1.0:
                tx_packet['vector']['z'] = -1.0
            elif directive_packet['is_rotated_left'] == 1.0:
                tx_packet['rotation']['z'] = 1.0
            elif directive_packet['is_rotated_right'] == 1.0:
                tx_packet['rotation']['z'] = -1.0

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
            default='movement/stabalization',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

