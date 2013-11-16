# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import math
import os
import sys
import time
from copy import deepcopy
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator


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

    The AI's job is to interpret sensor data and video information,
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
    com = Communicator(
            module_name=args.module_name,
            settings_path=args.settings_path)

    oligarchs = {
            "AdvisorsPeon": AdvisorsPeon(com),
            "PathOligargh": PathOligarch(com),
    }

    state = None
    last_sensor = {"timestamp": 0}
    last_video_front = {"timestamp": 0}
    last_video_down = {"timestamp": 0}
    last_advice = {"timestamp": 0}
    while True:
        #missive = deepcopy(missive_template)
        #com.publish_message(missive)

        # TODO: What will this be in the end?
        sensor = com.get_last_message("sensor/serial")
        if sensor and sensor['timestamp'] > last_sensor['timestamp']:
            last_sensor = sensor

        # TODO: Is this going to need more video streams?
        video = com.get_last_message("sensor/video")
        if video and video['timestamp'] > last_video['timestamp']:
            last_video = video

        advice = com.get_last_message("decision/advisor")
        if advice and advice['timestamp'] > last_advice['timestamp']:
            print advice["command"]
            if advice["command"] == "state: keyboard":
                state = 'keyboard'
            elif advice["command"] == "state: path full":
                state = 'path full'
            elif advice["command"] == "state: path orient":
                state = 'path orient'
            elif advice["command"] == "state: path follow":
                state = 'path follow'
            elif advice["command"] == "state: depth":
                state = 'depth'
            last_advice = advice

        if state == 'keyboard':
            decree = oligarchs["AdvisorsPeon"].decision(
                    last_sensor, last_video, last_advice)
        elif state == 'path full':
            decree = oligarchs["PathOligarch"].decision(
                    last_sensor, last_video)

        self.communicator.publish_message(decree)


        time.sleep(args.epoch)


def commandline():
    parser = argparse.ArgumentParser(
            description='Path following decision module.')
    parser.add_argument(
            '--settings_path', type=str,
            default=None,
            help='Settings file path.')
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

