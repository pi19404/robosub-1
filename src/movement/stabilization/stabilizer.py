# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import argparse
import time
import os
import sys
from copy import deepcopy
from random import random
from PID import PID_Controller
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator
from movement.settings import settings as settings

# Movement Decision Packet:
# {
#     "Task_AI_Movement":
#     {
#         "override":["up/down"] # override module
#         "forward/backward": 0.0,
#         "right/left": 0.0,
#         "up/down": 0.0,
#         "yaw": 0.0,
#         "roll": 0.0,
#         "pitch": 0.0
#     }
# }

# Sensor Packet Format: (from sensor/sanitation-main.py)
# sensors = {
# 	'gyroscope' : {
# 		'gx' : 1,
# 		'gy' : 2,
# 		'gz' : 3
# 	},
# 	'accelerometer' : {
# 		'ax' : 1,
# 		'ay' : 2,
# 		'az' : 3
# 	},
# 	'battery_voltage' :  {
# 		'voltage' : 5
# 	},
# 	'depth' : {
# 		'value' : 10
# 	},
# 	'compass' : {
# 		'direction' : 0
# 	}
# }

# Filtered orientation packet:
# {
#  "filtered_orientation":
#  {
#      "pitch": 0, # in degrees, range -180 - 180, pos up, neg down
#      "roll": 0 # in degrees, range -180 - 180, pos right, neg left
#      "yaw": 0 # in degrees, range -180 - 180, pos right, neg left
#  }
# }

def main(args):
    com = Communicator(module_name=args.module_name)

    # Initializing PIDs
    PID = {}
    for i in settings["movement/stabilization"]["PID_Settings"].keys ():
        # Setpoint is initalized to zero
        PID[i] = PID_Controller (settings["movement/stabilization"]["PID_Settings"][i]["kP"], settings["movement/stabilization"]["PID_Settings"][i]["kI"], settings["movement/stabilization"]["PID_Settings"][i]["kD"], settings["movement/stabilization"]["PID_Settings"][i]["Min_I"], settings["movement/stabilization"]["PID_Settings"][i]["Max_I"], 0.0)

    # Outgoing packet format
    outgoing_packet = {"forward/backward": 0.0, "right/left": 0.0, "up/down": 0.0, "yaw": 0.0, "pitch": 0.0, "roll": 0.0}

    last_timestamp = 0
    while True:
        ai_packet = com.get_last_message ("decision")

        # Get depth and orientation data.
        depth = com.get_last_message("datafeed/sanitized/depth")
        orientation = com.get_last_message("movement/orientation")

        # Update rate of the ai is arbitrary and variable. Stabilization should continue regardless of message availability.
        # If we get an ai_packet that is different check if the desired roll and depth are different. If they are update the 
        # controllers and continue stabilizing. If the packet is not different continue stabilizing.
        # If this does not get a packet continue to stabilize but pass the old packet values so that the movement
        # stack continues to run.
        if ai_packet and ai_packet["timestamp"] > last_timestamp:
            last_timestamp = ai_packet["timestamp"]

            # Override currently just outputs zero
            if "up/down" in ai_packet["Task_AI_Movement"]["override"]:
                outgoing_packet["up/down"] = 0.0
            if "roll" in ai_packet["Task_AI_Movement"]["override"]:
                outgoing_packet["roll"] = 0.0
                
            # Check if stabilization point needs to be changed. If yes: change it.
            if PID["depth"].setpoint != ai_packet["Task_AI_Movement"]["depth"]:
                PID["depth"].set_setpoint (ai_packet["Task_AI_Movement"]["depth"])
            if PID["roll"].setpoint != ai_packet["Task_AI_Movement"]["roll"]:
                PID["roll"].set_setpoint (ai_packet["Task_AI_Movement"]["roll"])


        if depth:
            outgoing_packet["up/down"] = PID["depth"].compute (depth["value"])
        if orientation:
            outgoing_packet["roll"] = PID["roll"].compute (orientation["roll"])

        if ai_packet:
            # These values are not being stabilized
            outgoing_packet["forward/backward"] = ai_packet["Task_AI_Movement"]["forward/backward"]
            outgoing_packet["right/left"] = ai_packet["Task_AI_Movement"]["right/left"]
            outgoing_packet["yaw"] = ai_packet["Task_AI_Movement"]["yaw"]
            outgoing_packet["pitch"] = ai_packet["Task_AI_Movement"]["pitch"]

        # Add name
        stabilization_packet = {"Stabilization_to_Fuzzification": outgoing_packet}
        com.publish_message(stabilization_packet)

        time.sleep(args.epoch)

def commandline():
    parser = argparse.ArgumentParser(description='Stabilization module.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.02,
            help='Sleep time per cycle.')
    parser.add_argument('-m', '--module_name', type=str,
            default='movement/stabilization',
            help='Module name.')
    return parser.parse_args()

if __name__ == '__main__':
    args = commandline()
    main(args)
