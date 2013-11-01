from subprocess import check_output
import argparse
import json
import time

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-m', '--module', type=str,
            help='Module to watch.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    return parser.parse_args()


def main(args):
    # This is horrible... but whatever.
    while True:
        last_msg = None
        with open('/tmp/robosub/log.out', 'r') as fin:
            for line in fin.readlines():
                msg = json.loads(line)
                if not last_msg:
                    last_msg = msg
                if (msg and
                    msg['module_name'] == args.module and
                    msg['timestamp'] > last_msg['timestamp']):
                    last_msg = msg
                    print msg
        time.sleep(args.epoch)


if __name__ == '__main__':
    main(commandline())


