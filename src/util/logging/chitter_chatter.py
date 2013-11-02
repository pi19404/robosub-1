from subprocess import check_output
import argparse
import json
import time
from pprint import pprint

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-m', '--module', type=str,
            help='Module to watch.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    return parser.parse_args()


def main(args):
    with open('/tmp/robosub/log.out', 'r') as fin:
        last_msg = {'message_number': 0}
        while True:
            try:
                msg = json.loads(fin.readline())
            except ValueError:
                # This fails for a few reasons. The main reason is that the msg
                # is '' and the error will be "No JSON object could be
                # decoded". At least one more source of a ValueError exists.
                time.sleep(args.epoch)
                continue
            if (msg and
                msg['module_name'] == args.module and
                msg['message_number'] > last_msg['message_number']):
                last_msg = msg
                pprint(msg, width=1)
            else:
                pass


if __name__ == '__main__':
    main(commandline())


