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
            comm_json_path=args.comm_settings)

    while True:
        com.publish_message({'message': None})
        time.sleep(args.epoch)
        for mname in com.listening():
            recv_msg = com.get_last_message(mname)
            if recv_msg:
                print recv_msg

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-c', '--comm_settings', type=str,
            default=None,
            help='Communication file path.')
    parser.add_argument('-e', '--epoch',  type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/directive',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    print args
    main(args)

