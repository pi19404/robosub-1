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
| q     | w     | e     | r     |
|       |       |       |       |
| QUIT  |FORWARD|  N/A  | RISE  |
++------++------++------++------++
 | a     | s     | d     | f     |
 | ROTATE|       | ROTATE|       |
 | LEFT  |  BACK | RIGHT | FALL  |
 +-------+-------+-------+-------+
"""
    com = Communicator(
            module_name=args.module_name,
            settings_path=args.settings_path)

    advice_template = {"command": None}

    while True:
        key = getch()
        print "{0}\r".format(key),
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
            advice["command"] = "state: path full"
        elif key == '3':
            advice["command"] = "state: path orient"
        elif key == '4':
            advice["command"] = "state: path follow"
        elif key == '5':
            advice["command"] = "state: depth"
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

        com.publish_message(advice)

def commandline():
    parser = argparse.ArgumentParser(description='Keyboard advice module.')
    parser.add_argument(
            '--settings_path', type=str,
            default=None,
            help='Settings file path.')
    parser.add_argument(
            '-m', '--module_name', type=str,
            default='decision/advisor',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

