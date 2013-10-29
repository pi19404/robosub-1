# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""
Daniel Retherford
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
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


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


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()


getch = _Getch()


def main(args):
    print """Usage:
+-------+-------+-------+-------+
| q     | w     | e     | r     |
|       |       |       |       |
| QUIT  |FORWARD|  N/A  | RISE  |
++------++------++------++-----++
 | a     | s     | d     | f     |
 | ROTATE|       | ROTATE|       |
 | LEFT  |  BACK | RIGHT | FALL  |
 +-------+-------+-------+-------+
"""
    com = Communicator(
            module_name=args.module_name,
            settings_path=args.settings_path)

    # x is left/right
    # y is forward/backward
    # z is up/down
    missive_template = {
        "desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
        "desired_orientation": {"x": 0.0, "y": 0.0, "z": 0.0},
        "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0}
    }

    while True:
        key = ord(getch())
        missive = deepcopy(missive_template)

        if key == ord('w'):
            missive['desired_offset']['y'] = 9001.0
            missive['desired_velocity']['y'] = 1.0
        elif key == ord('a'):
            missive['desired_orientation']['z'] = 3 * math.pi / 2
        elif key == ord('s'):
            missive['desired_offset']['y'] = -9001
            missive['desired_velocity']['y'] = -1.0
        elif key == ord('d'):
            missive['desired_orientation']['z'] = math.pi / 2
        elif key == ord('r'):
            missive['desired_offset']['z'] = 9001
            missive['desired_velocity']['z'] = 1.0
        elif key == ord('f'):
            missive['desired_offset']['z'] = -9001
            missive['desired_velocity']['z'] = -1.0
        elif key == ord('e'):
            # This will turn off the motors.
            pass
        elif key == ord('q'):
            sys.exit()

        com.publish_message(missive)

def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('--settings_path', type=str,
            default=None,
            help='Settings file path.')
    parser.add_argument('-m', '--module_name', type=str,
            default='decision',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

