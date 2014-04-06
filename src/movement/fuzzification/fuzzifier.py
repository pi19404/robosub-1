# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

from copy import deepcopy
from random import random
import argparse
import json
import os
import sys
import time
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator
from robosub_settings import settings

#################################################################
# TODO: Figure out name of AI module and desired packet format. #
# This module is unusable until then.                           #
#################################################################

def get_membership(value, set_points):
    """Returns the membership quantity for a value in a fuzzy set.

    >>> is_rotated_right_points = [(3.13, 0.0), (3.14, 1.0),
    ...                            (4.71, 1.0), (6.28, 0.0)]
    >>> get_membership(3.14, is_rotated_right_points)
    1.0
    >>> get_membership(0.0, is_rotated_right_points)
    0.0
    >>> print("{0:.4}".format(get_membership(5.0, is_rotated_right_points)))
    0.8153
    >>> print("{0:.4}".format(get_membership(6.0, is_rotated_right_points)))
    0.1783
    >>> get_membership(0.8, [(0.0, 0.0), (1.0, 1.0)])
    0.8
    >>> get_membership(0.2, [(1.0, 1.0), (0.0, 0.0)])
    0.2
    >>> get_membership(9001, [(-2.0, 1.0), (0.0, 0.0)])
    0.0

    """
    set_points = sorted(set_points, cmp=lambda a, b: cmp(a[0], b[0]))
    if value <= set_points[0][0]:
        return set_points[0][1]
    if set_points[-1][0] <= value:
        return set_points[-1][1]

    idx = 0
    while True:
        if set_points[idx][0] <= value and value < set_points[idx + 1][0]:
            break
        idx += 1
    # How far along this interval are we?
    proportion = ((value - set_points[idx][0]) /
                  (set_points[idx + 1][0] - set_points[idx][0]))
    return (set_points[idx][1] +
            proportion * (set_points[idx + 1][1] - set_points[idx][1]))

def main(args):
    com = Communicator(module_name=args.module_name)
    fuzzy_sets = settings[args.module_name]["fuzzy_sets"]

    # These values represent the submarine's membership in 8 fuzzy sets.
    # These sets come in pairs (left/right, back/forward, etc.) and represent
    # where the submarine is in relation to where it wants to be.
    # The roll and yaw sets are not available to directive module since
    # they should be controlled by either the stabilization module, or by
    # the awesome balancing skills of the mech-e's.

    # Expected packet sent by this module:
    #   packet = {
    #           'is_left': 0.0,
    #           'is_right': 0.0,
    #           'is_back': 0.0,
    #           'is_forward': 0.0,
    #           'is_low': 0.0,
    #           'is_high': 0.0,
    #           'is_rotated_left': 0.0,
    #           'is_rotated_right': 0.0}

    # Received missive is of the form:
    #   {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
    #    "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
    #    "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0},
    #    "face_of_power": self.face_of_power,
    #    "timestamp": time.time()}

    last_timestamp = 0.0
    while True:
        missive = com.get_last_message("movement/stabalization")

        # TODO: Fix packet name
        if missive and missive['timestamp'] > last_timestamp:
            last_timestamp = missive['timestamp']
            packet = {}
            packet['is_left'] = get_membership(
                    missive['desired_offset']['x'], fuzzy_sets['is_left'])
            packet['is_right'] = get_membership(
                    missive['desired_offset']['x'], fuzzy_sets['is_right'])
            packet['is_back'] = get_membership(
                    missive['desired_offset']['y'], fuzzy_sets['is_back'])
            packet['is_forward'] = get_membership(
                    missive['desired_offset']['y'], fuzzy_sets['is_forward'])
            packet['is_low'] = get_membership(
                    missive['desired_offset']['z'], fuzzy_sets['is_low'])
            packet['is_high'] = get_membership(
                    missive['desired_offset']['z'], fuzzy_sets['is_high'])
            packet['is_rotated_left'] = get_membership(
                    missive['desired_orientation']['yaw'],
                    fuzzy_sets['is_rotated_left'])
            packet['is_rotated_right'] = get_membership(
                    missive['desired_orientation']['yaw'],
                    fuzzy_sets['is_rotated_right'])
            com.publish_message(packet)
        time.sleep(args.epoch)


def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/fuzzification',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)

