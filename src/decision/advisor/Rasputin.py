# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""
RoboSub Movement Program
"""

# TODO
# Maintain depth check, control power to motors, Add ability to incremental
# power increase/decrease, improve the shut off function

import argparse
import math
import os
import sys
import time
from copy import deepcopy
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator


class _Getch:
    """Gets a single character from standard input.

    Does not echo to the screen.

    Source: ("http://code.activestate.com/recipes/"
             "134892-getch-like-unbuffered-character-reading-from-stdin/")

    Liscense: PSF
    Created by Danny Yoo on Fri, 21 Jun 2002

    """
    def __init__(self):
        self.impl = _GetchUnix()

    def __call__(self):
        return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


getch = _Getch()


def main(args):
    print """Usage:
   +-------+-------+-------+-------+
   | 1     | 2     | 3     | 4     |        magnitude will always be between
   |       |       |       |       |        0 and 127. Use '[' to decrease the
   |KEYBOARD PATH  | DEPTH | N/A   |        magnitude and ']' to increase it.
   ++------++------++------++------++
    | q     | w     | e     | r     |
    |       |       |       |       |
    | QUIT  |FORWARD| STOP  | RISE  |
    ++------++------++------++------++
     | a     | s     | d     | f     |
     | ROTATE|       | ROTATE|       |
     | LEFT  |  BACK | RIGHT | FALL  |
     +-------+-------+-------+-------+
"""
    com = Communicator(module_name=args.module_name)

    advice_template = {"command": None}
    magnitude = 91

    while True:
        key = getch()
        print "magnitude: {0:3} key: {1}\r".format(magnitude, key),
        advice = deepcopy(advice_template)

        if key == 'w':
            advice["command"] = "forward"
        elif key == 'a':
            advice["command"] = "yaw left"
        elif key == 's':
            advice["command"] = "backward"
        elif key == 'd':
            advice["command"] = "yaw right"
        elif key == 'r':
            advice["command"] = "rise"
        elif key == 'f':
            advice["command"] = "descend"
        elif key == 't':
            advice["command"] = "roll left"
        elif key == 'g':
            advice["command"] = "roll right"
        elif key == 'e':
            advice["command"] = "stop"
        elif key == '1':
            advice["command"] = "state: keyboard"
        elif key == '2':
            advice["command"] = "state: path"
        elif key == '3':
            advice["command"] = "state: depth"
        elif key == '[':
            magnitude -= 1
        elif key == ']':
            magnitude += 1

       #elif key == '6':
       #    advice["command"] =
       #elif key == '7':
       #    advice["command"] =
       #elif key == '8':
       #    advice["command"] =
       #elif key == '9':
       #    advice["command"] =
        elif key == 'q':
            sys.exit()

        if magnitude > 127:
            magnitude = 127
        if magnitude < 0:
            magnitude = 0

        advice["magnitude"] = magnitude

        com.publish_message(advice)

def commandline():
    parser = argparse.ArgumentParser(description='Keyboard advice module.')
    parser.add_argument(
            '-m', '--module_name', type=str,
            default='decision/advisor',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

