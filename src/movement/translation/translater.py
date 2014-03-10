# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
from copy import deepcopy
from random import random
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator
from robosub_settings import settings

# TODO: This module should take the fuzzy sets produced by
# movement/stabilization and should translate them into raw digital
# values that can be sent over the serial interface.
# microcontroller_interface.py currently does much of this processing,
# but it shouldn't. microcontroller_interface.py should figure out how
# to send data over the serial interface and how to receive data over
# the serial interface. Anything that is beyond that scope, such as
# translating a magnitude into a raw value, should be moved into this
# module.

def translate(motor, defuzzified_value):
    """Translate motor thrust percentage to 8-bit value. """

    if defuzzified_value == 0.0:
        return 0

    motor_range = settings['movement/translation']['thresholds'][motor]
    if defuzzified_value >= 0:
        low = motor_range['positive'][0]
        high = motor_range['positive'][1]
    else:
        low = -motor_range['negative'][0]
        high = -motor_range['negative'][1]
    absdif = abs(high - low)

    # Return the equivalent 8-bit motor value for defuzzified value.
    return (int(defuzzified_value * absdif) + low) * motor_range['multiplier']

def main(args):
    com = Communicator("movement/translation")

    last_packet_time = 0.0
    while True:
        rx_packet = com.get_last_message("movement/defuzzification")
        if rx_packet and rx_packet['timestamp'] > last_packet_time:
            tx_packet = {'Thruster_Values': {}}
            last_packet_time = rx_packet['timestamp']
            for k in rx_packet['Defuzzified_Sets']:
                tx_packet['Thruster_Values'][k] = \
                        translate(k, rx_packet['Defuzzified_Sets'][k])
            com.publish_message(tx_packet)
            print tx_packet

        time.sleep(args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

