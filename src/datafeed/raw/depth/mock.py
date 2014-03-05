# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
import argparse
import os
import sys
import time
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator

def commandline():
    parser = argparse.ArgumentParser(description='Sanitization module.')
    parser.add_argument(
            '-e', '--epoch', type=float, default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('--depth', type=int, default=500)
    return parser.parse_args()

def main(args):
    com=Communicator("datafeed/raw/depth")
    while True:
        com.publish_message({"DEPTH": args.depth})
        time.sleep(args.epoch)

if __name__ == '__main__':
    main(commandline())

