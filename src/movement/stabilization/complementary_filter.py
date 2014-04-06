# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
from copy import deepcopy
from random import random
from time import sleep, time
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

# Sensor Packet Format: (from sensor/sanitation-main.py)
# sensors = {
#   'gyroscope' : {
#       'gx' : 1,
#       'gy' : 2,
#       'gz' : 3
#   },
#   'accelerometer' : {
#       'ax' : 1,
#       'ay' : 2,
#       'az' : 3
#   },
#   'battery_voltage' :  {
#       'voltage' : 5
#   },
#   'depth' : {
#       'value' : 10
#   },
#   'compass' : {
#       'direction' : 0
#   }
# }

# TODO: Needs to go in settings file
Settings = {"accel_constant": .95, "gyro_constant": .05}

def main(args):
    com = Communicator(module_name=args.module_name)

    last_accel_timestamp = 0
    last_gyro_timestamp = 0

    raw_gyro_pitch = 0
    raw_gyro_roll = 0
    raw_gyro_yaw = 0

    raw_accel_pitch = 0
    raw_accel_roll = 0

    filtered_orientation = {"pitch": 0, "roll": 0, "yaw": 0}

    while True:
        accel = com.get_last_message("datafeed/sanitized/accelerometer")
        gyro = com.get_last_message("datafeed/sanitized/gyroscope")
        compass = com.get_last_message("datafeed/sanitized/compass")

        # Update gyro measurements
        if gyro and gyro["timestamp"] > last_gyro_timestamp:
            dt = gyro["timestamp"] - last_gyro_timestamp
            last_gyro_timestamp = gyro_timestamp

            raw_gyro_pitch = raw_gyro_pitch + (gyro["gy"] * dt)
            raw_gyro_roll = raw_gyro_roll + (gyro["gx"] * dt)
            # This will be very innacurate right now
            raw_gyro_yaw = raw_gyro_yaw + (gyro["gz"] * dt)

        # Update accel measurements
        if accel and accel["timestamp"] > last_accel_timestamp:
            last_accel_timestamp = accel_timestamp

            raw_accel_pitch = atan2 (accel["ax"], accel["az"])
            raw_accel_roll = atan2 (accel["ay"], accel["az"])
        
        # Perform filtering
        filtered_orientation["pitch"] = ((Settings["gyro_constant"]) * raw_gyro_pitch) + ((1 - Settings["accel_constant"]) * raw_accel_pitch)
        filtered_orientation["roll"] = ((Settings["gyro_constant"]) * raw_gyro_roll) + ((1 - Settings["accel_constant"]) * raw_accel_roll)
        filtered_orientation["yaw"] = raw_gyro_yaw
        

def commandline():
    parser = argparse.ArgumentParser(description='Filtering module for orientation.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/orientation',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
