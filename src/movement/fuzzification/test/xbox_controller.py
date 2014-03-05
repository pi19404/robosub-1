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
    controller =  Controller (["X1", "Y1", "X2", "Y2", "R2", "L2"], ["right/left", "forward/backward", "yaw", "pitch", "up", "down"])

    while True:
        packet = controller.get_values ()
        print packet
        com.publish_message (packet)
        sleep (args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.5,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/fuzzification',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
