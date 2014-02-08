# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
from copy import deepcopy
from random import random
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

def main(args):
    com = Communicator(module_name=args.module_name)

    # TODO We need to settle on a good convention for these vectors.
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
            'rotation': {'yaw': 0.0, 'pitch': 0.0, 'roll': 0.0}}

    last_packet_time = 0.0
    while True:
        directive_packet = com.get_last_message("movement/directive")
        if directive_packet and directive_packet['timestamp'] > last_packet_time:
            last_packet_time = directive_packet['timestamp']

            # Kludges to handle keyboard inputs.
            tx_packet = deepcopy(packet)
            if directive_packet['is_left'] > 0.0:
                tx_packet['vector']['x'] = 1.0
            elif directive_packet['is_right'] > 0.0:
                tx_packet['vector']['x'] = -1.0
            elif directive_packet['is_back'] > 0.0:
                tx_packet['vector']['y'] = 1.0
            elif directive_packet['is_forward'] > 0.0:
                tx_packet['vector']['y'] = -1.0
            elif directive_packet['is_low'] > 0.0:
                tx_packet['vector']['z'] = 1.0
            elif directive_packet['is_high'] > 0.0:
                tx_packet['vector']['z'] = -1.0
            elif directive_packet['is_rotated_left'] > 0.0:
                tx_packet['rotation']['yaw'] = 1.0
            elif directive_packet['is_rotated_right'] > 0.0:
                tx_packet['rotation']['yaw'] = -1.0

            com.publish_message(tx_packet)

        time.sleep(args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/stabilization',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

