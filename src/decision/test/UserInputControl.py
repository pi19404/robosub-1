# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""
Daniel Retherford
RoboSub Movement Program
"""

# TODO
# Maintain depth check, control power to motors, Add ability to incremental
# power increase/decrease, improve the shut off function

import sys
import time

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


def userChangeDepth(keyPress):
    """Controls the depth of the robo sub"""
    x = 0
    speed = 2.0
    speedAdjustment = 2.3
    if keyPress == "down":
        # Move sub down
        for x in range(0, 3):
            print("Sub moving down at", speed)
            speed = speed + speedAdjustment
            time.sleep(1)
    elif keyPress == "up":
        # Move sub up
        for x in range(0, 3):
            print("Sub moving up at", speed)
            speed = speed + speedAdjustment
            time.sleep(1)

    speed = 2.0 # set back to original value
    return

def userForwardBackwardChange(keyPress):
    """Controls the forward and backwards movement of the sub"""
    if keyPress == "forward":
        print("Sub is moving forward at")
    elif keyPress == "backwards":
        print("Sub is moving backwards at")
    return

def userRotationChange(keyPress):
    """Controls the rotational movement left and right of the sub"""
    if keyPress == "left":
        print("Sub is rotating to the left at")
    elif keyPress == "right":
        print("Sub is rotating to the right at")

def main():
    print """Usage:
+-------+-------+-------+-------+
| q     | w     | e     | r     |
|       |       |       |       |
| QUIT  |FORWARD|  N/A  | RISE  |
++------++------++------++-----++
 | a     | s     | d     | f     |
 | ROTATE|       | ROTATE|       |
 | LEFT  |  BACK | RIGHT | FALL  |
 +-------+-------+--=----+-------+
"""

    while True:
        # Used to capture the keypress
        key = ord(getch())
        print key

        if key == ord('w'):
            userForwardBackwardChange("forward")
        elif key == ord('a'):
            userRotationChange("left")
        elif key == ord('s'):
            userForwardBackwardChange("backwards")
        elif key == ord('d'):
            userRotationChange("right")
        elif key == ord('r'):
            userChangeDepth("up")
        elif key == ord('f'):
            userChangeDepth("down")
        elif key == ord('q'):
            print("sub motors shutting down")
            sys.exit()

if __name__ == "__main__":
    main()

