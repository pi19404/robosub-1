from xboxdrv_parser import Controller
from time import sleep
import argparse
import os
import sys
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator
from robosub_settings import settings

def main (args):
    com = Communicator (args.module_name)
    controller = Controller (["X1", "Y1", "X2", "Y2", "R2", "L2"], ["right/left", "forward/backward", "yaw", "pitch", "up", "down"], (0, 255), (-1, 1))

    while True:
        control_packet = controller.get_values ()

        try:
            outgoing_packet = {"right/left": 0.0, "forward/backward": 0.0, "yaw": 0.0, "pitch": 0.0, "up/down": 0.0, "roll": 0.0}
            # Further parse controller values here
            # Controller's sticks Y axis are switched
            control_packet["forward/backward"] = -control_packet["forward/backward"]
            control_packet["pitch"] = -control_packet["pitch"]

            # Up and Down are not -1 to 1. Just 0 - 1
            control_packet["up"] = controller.map_range(control_packet["up"], -1, 1, 0, 1)
            control_packet["down"] = controller.map_range(control_packet["down"], -1, 1, 0, -1)
            
            # Transferring to outgoing packet
            outgoing_packet["forward/backward"] = control_packet["forward/backward"]
            outgoing_packet["right/left"] = control_packet["right/left"]
            outgoing_packet["up/down"] = control_packet["up"] + control_packet["down"]
            outgoing_packet["yaw"] = control_packet["yaw"]
            outgoing_packet["pitch"] = control_packet["pitch"]
            #outgoing_packet["roll"] = control_packet["roll"]
            outgoing_packet["roll"] = 0.0

            # Controller sticks are not centered very well. 
            # TODO: Find a better way to do this (short of getting a new controller)
            for key in outgoing_packet.keys ():
                if abs (outgoing_packet[key]) < .10: outgoing_packet[key] = 0.0

            print outgoing_packet

            Fuzzy_Sets = {"Fuzzy_Sets": outgoing_packet}
            com.publish_message (Fuzzy_Sets)

        except KeyError as i:
            pass

        sleep (args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.1,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/fuzzification',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
