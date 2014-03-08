#COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""
Logger Module
    This listens to the various sensors and modules and outputs a log for each

Sensors this module listens to can be found in robosub/src/settings.json

"""

from time import sleep
import argparse
import os
import json
import sys
import time
from subprocess import check_call, CalledProcessError
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator  #preffered method

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument(
            '-e', '--epoch',  type=float, default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument(
            '-o', '--output', type=str, default="/home/robosub/logging/log.out",
            help='Location of the log file.')
    return parser.parse_args()

def main(args):
    com = Communicator(module_name="util/logger")
    com.publish_message("logger.py started")

    try:
        check_call(['mv', args.output,
                    "/home/robosub/logging/log.out." + str(time.time())])
    except CalledProcessError:
        pass

    with open(args.output, 'w') as log_file:
        while True:
            for mname in com.listening():
                for message in com.get_messages(mname):
                    json.dump(message, log_file)
                    log_file.write('\n')
            log_file.flush()
            sleep(args.epoch)

    log_file.close()


if __name__ == "__main__":
    args = commandline()
    main(args)

