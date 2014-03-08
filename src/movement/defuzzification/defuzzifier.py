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

    #incoming_packet = {"forward/backward": 0.0, "right/left": 0.0, "up/down": 0.0, "yaw": 0.0, "roll": 0.0, "pitch": 0.0}

    last_packet_time = 0
    while True:
        incoming_packet = com.get_last_message("movement/fuzzification")
        if incoming_packet and incoming_packet['timestamp'] > last_packet_time:
            last_packet_time = incoming_packet['timestamp']

            try:
                outgoing_packet = {"front_left": 0.0, "front_right": 0.0, "middle_left": 0.0, "middle_right": 0.0, "back_left": 0.0, "back_right": 0.0}

                # Possible TODO: Scale all values except for forward/backward by some constant so yaw, strafe, etc, are easier to control
                outgoing_packet["front_left"] = incoming_packet["Fuzzy_Sets"]["forward/backward"] + incoming_packet["Fuzzy_Sets"]["right/left"] + incoming_packet["Fuzzy_Sets"]["yaw"]
                outgoing_packet["front_right"] = incoming_packet["Fuzzy_Sets"]["forward/backward"] - incoming_packet["Fuzzy_Sets"]["right/left"] - incoming_packet["Fuzzy_Sets"]["yaw"]

                outgoing_packet["back_left"] = incoming_packet["Fuzzy_Sets"]["forward/backward"] - incoming_packet["Fuzzy_Sets"]["right/left"] + incoming_packet["Fuzzy_Sets"]["yaw"]
                outgoing_packet["back_right"] = incoming_packet["Fuzzy_Sets"]["forward/backward"] + incoming_packet["Fuzzy_Sets"]["right/left"] - incoming_packet["Fuzzy_Sets"]["yaw"]

                outgoing_packet["middle_left"] = incoming_packet["Fuzzy_Sets"]["up/down"] + incoming_packet["Fuzzy_Sets"]["roll"]
                outgoing_packet["middle_right"] = incoming_packet["Fuzzy_Sets"]["up/down"] - incoming_packet["Fuzzy_Sets"]["roll"]

                for key in outgoing_packet.keys():
                    if outgoing_packet[key] > 1.0: outgoing_packet[key] = 1.0
                    if outgoing_packet[key] < -1.0: outgoing_packet[key] = -1.0

                print outgoing_packet

                com.publish_message({"Defuzzified_Sets": outgoing_packet})
            except KeyError as e:
                pass

        time.sleep(args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/defuzzification',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

