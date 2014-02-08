# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import math
import os
import sys
import time
from copy import deepcopy
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator
from advisors_peon import AdvisorsPeon
from path_oligarch import PathOligarch
from depth_oligarch import DepthOligarch

def choose_last_packet(com, module, last_packet):
    packet = com.get_last_message(module)
    if (packet and
        (not last_packet or packet['timestamp'] > last_packet['timestamp'])):
        ret_packet = packet
        success = True
    else:
        ret_packet = last_packet
        success = False

    return ret_packet, success

def main(args):
    """The resultant command is communicated over the grapevine in a
    dicitonary with this format:

    {"desired_offset":
        {"x": 0.0,
         "y": 0.0,
         "z": 0.0},
     "desired_orientation":
        {"yaw": 0.0,
         "pitch": 0.0,
         "roll": 0.0},
     "desired_velocity":
        {"x": 0.0,
         "y": 0.0,
         "z": 0.0}
    }

    yaw, pitch, and roll are in radians. A small positive yaw specifies
    a desire to turn right, a small possitive pitch specifies a desire
    to tilt the nose up, and a small positive roll specifies a desire
    to lower the right side of the sub.

    The AI's job is to interpret sensor data and vision information,
    and from that, specify where the sub SHOULD be relative to where
    it CURRENTLY is. The desired location represents a state that
    should be achieved at some future time.

    The vector "desired_offset" specifies the relative location of
    that desired state, the "desired_orientation" specifies which
    direction the submarine should be facing, and the "desired_velocity"
    specifies how the submarine should be moving at that point.

    This information structure intentionally doesn't allow the AI to specify
    the "roll" axis or a desired rotational velocity.

    One way to think about this is that the AI module needs to figure out
    what the next waypoint is, and where that waypoint is relative to
    where the submarine currenlty is stationed.

    This module should be something of a state machine. Given a specific
    state, one of the Oligarch classes should be able to decide what to
    do, but something needs to decide which oligarch is in charge.

    """
    com = Communicator(module_name=args.module_name)

    oligarchs = {
            "AdvisorsPeon": AdvisorsPeon(com),
            "PathOligarch": PathOligarch(com),
            "DepthOligarch": DepthOligarch(com),
    }

    state = None
    vision_cam_front = None
    vision_cam_down = None
    advice = None
    depth = None
    while True:
        advice, success = choose_last_packet(com, "decision/advisor", advice)
        if success:
            if advice["command"] == "state: keyboard":
                state = 'keyboard'
            elif advice["command"] == "state: path":
                state = 'path'
            elif advice["command"] == "state: depth":
                state = 'depth'
            elif advice["command"] == "stop":
                state = "stop"

        vision_cam_front, _ = choose_last_packet(
                com, "sensor/vision/cam_front", vision_cam_front)
        vision_cam_down, _ = choose_last_packet(
                com, "sensor/vision/cam_down", vision_cam_front)
        depth, _ = choose_last_packet(com, "sensor/depth", depth)

        if state == "stop":
            pass
        elif state == 'keyboard' and success:
            oligarchs["AdvisorsPeon"].decision(advice)
        elif state == 'path':
            oligarchs["PathOligarch"].decision(vision_cam_front, vision_cam_down)
        elif state == 'depth':
            print "state: depth"
            oligarchs["DepthOligarch"].decision(depth)

        time.sleep(args.epoch)


def commandline():
    parser = argparse.ArgumentParser(
            description='Path following decision module.')
    parser.add_argument(
            '-m', '--module_name', type=str,
            default='decision',
            help='Module name.')
    parser.add_argument(
            '-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

