# If a top level key is associated with a dictionary and that nested
# dictionary does NOT have "not_a_module" as one of its keys, then the
# dictionary must specify settings for a module.
#
# The jumpstart program (currently "Dr. Frankenstein's Machine") will look
# for the keys "release" and "mock". If these keys exist, they must be
# associated with a dictionary. THAT dictionary may have the keys "name" and
# "args". "name" must specify the program name while "args", if it is present,
# must specify in a list the arguments that are needed to start the
# program.

# This file should be somewhere on your PYTHONPATH. Dr. Frankenstein's Machine
# will set that for you, but if you launch a program manually, it could be
# a hassle.

import os

# Maximum time a program might wait before checking for input.
EPOCH = "0.05"

settings = {
    "publisher_high_water_mark": 1024,
    "publisher_buffer_length": 1024,
    "sensor/vision/cam_front": {
        "symlink": "/dev/cam_front",
        "codec": "MJPG",
        "log": ["raw", "processed"],
        "stream": {
        },
        "fps": 30,
        "width": 640,
        "height": 480,
        "plugins": ["tollbooth"],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/vision/cam_down": {
        "symlink": "/dev/cam_down",
        "camera": "down",
        "enabled": False,
        "dev": 1,
        "codec": "MJPG",
        "log": ["raw", "processed"],
        "stream": {
        },
        "fps": 30,
        "width": 640,
        "height": 480,
        "plugins": ["PathDetection"],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/vision/fates": {
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None},
        "maintenance_interval": 5,
        "vision_processors": [
            "sensor/vision/cam_front",
            "sensor/vision/cam_down"
        ]
    },
    "util/logger": {
        # The "listen" value is populated at the end of this file.
        "listen": [],
        "release": {
            "name": "logger.py",
            "args": ["--epoch", 0.005]},
        "mock": {
            "name": "logger.py",
            "args": ["--epoch", 0.005]}
    },
    "movement/fuzzification": {
        #"ip": "192.168.1.7", # IP address of computer with ps3 controller
        "listen": [
            "movement/translation",
            "decision",
            "sensor/vision/cam_front",
            "sensor/vision/cam_down"
        ],
        # The N values in each fuzzy set will define N + 1 regions.
        # The first number represents an x value whlie the second number
        # represents a y value. All points left of the lowest x value will have
        # the same y value. The regions between specified points will have
        # a value determined by connecting a straight line between those points
        # All points to the right of the highest value have the same y value.
        # For example, the values
        # [[-3.0, 1.0], [-1.0, 0.5], [0.0, 0.0], [2.0, 1.0], [3.0, 0.0]] will
        # look (something) like:
        #-----------\                     1.0           /                      
        #            ---\                 |            / \                     
        #                ---\             |          /    \                    
        #                    ---\         |         /      \                   
        #                        ---\     0.5     /         \                  
        #                            \    |      /           \                 
        #                             \   |    /              \                
        #                              \  |   /                \               
        #                               \ | /                   \              
        #        ------------------------\-/---------------------\-------------
        #-4.0    -3.0    -2.0    -1.0     0.0    1.0    2.0    3.0
        "fuzzy_sets": {
            "is_right": [[-2.0, 1.0], [0.0, 0.0]],
            "is_left": [[0.0, 0.0], [2.0, 1.0]],
            "is_forward": [[-2.0, 1.0], [0.0, 0.0]],
            "is_back": [[2.0, 1.0], [0.0, 0.0]],
            "is_low": [[2.0, 1.0], [0.0, 0.0]],
            "is_high": [[-2.0, 1.0], [0.0, 0.0]],
            "is_rotated_right":
                [[3.13, 0.0], [3.14, 1.0], [4.712, 1.0], [6.28, 0.0]],
            "is_rotated_left":
                [[0.0, 0.0], [1.57, 1.0], [3.12, 1.0], [3.13, 0.0]]
        },
        "release": {
            "name": "fuzzy_logic_fuzzifier.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "/test/xbox_controller.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/defuzzification": {
        "listen": [
            "movement/fuzzification",
            "datafeed/raw/accelerometer"
        ],
        "release": {
            "name": "defuzzifier.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "slider1.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/translation": {
        #"ip": "192.168.1.7", # IP address of person with slider control
        "listen": [
            "movement/defuzzification"
        ],
        "thresholds": {
            "front_left": {
                # TODO we may need to specify different numbers for forward vs
                # backward.
                "positive": [1, 127],
                "negative": [1, 127],
                # Multiplier may flip the motor direction for motors that are
                # wired backwards. Note that multiplier is applied before the
                # positive and negative thresholds are applied, so positive and
                # negative are the same direction for each thruster regardless
                # of wiring.
                "multiplier": -1
            },
            "front_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "middle_left": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "middle_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "back_left": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": 1
            },
            "back_right": {
                "positive": [1, 127],
                "negative": [1, 127],
                "multiplier": -1
            }
        },
        "release": {
            "name": "translater.py",
            "args": ["--epoch", EPOCH]
        },
        "mock": {
            "name": "slider2.py",
            "args": ["--epoch", EPOCH]
        },
    },
    # TODO: The commandline arguments are quite meaningful here, so they
    # should have something of a comment.
    "microcontroller": {
        "listen": [
            "decision/advisor",
            "movement/translation",
        ],
        "release": {
            "name": "microcontroller_interface.py",
            "args": [
                "--epoch", "0.01",
                "--baudrate", "56818",
                "--port", '/dev/ttyUSB0',
                #"--magnitude", "100",
            ]
        },
        "mock": {
            "name": "microcontroller_interface.py",
            "args": [
                "--epoch", "0.01",
                "--debug",
                "--baudrate", "56818",
                "--disable_depth",
                #"--magnitude", "100"
            ]
        },
    },
    "decision": {
        "listen": [
          "decision/advisor",
          "sensor/vision/cam_front",
          "sensor/vision/cam_down",
          "datafeed/sanitized/accelerometer",
          "datafeed/sanitized/gyroscope",
          "datafeed/sanitized/compass",
          "datafeed/sanitized/depth",
          # XXX: Remove "datafeed/raw/depth".
          # This is for initial calibration of the depth only.
          "datafeed/raw/depth",
        ],
        "release": {
            "name": "ai_state_machine.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "ai_state_machine.py",
            "args": ["--epoch", EPOCH]},
        "depth_threshold": 550
    },
    "decision/advisor": {
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "util/vision_viewer": {
        "listen": [
            "sensor/vision/cam_front",
            "sensor/vision/cam_down"
        ],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/sanitation": {
        "listen": [
            "datafeed/raw/depth",
            "datafeed/raw/battery_voltage",
            "datafeed/raw/accelerometer",
            "datafeed/raw/gyroscope",
            "datafeed/raw/compass"
        ],
        "release": {
            "name": "sanitation.py",
            "args": [
                "--calibration_samples", 10]
        },
        "mock": {
            "name": "sanitation.py",
            "args": [
                "--debug",
                "--calibration_samples", 10]}
    },
    # The datafeed "modules" represent the data at its various stages, and
    # unless a mock module is faking a stage of the data pipeline, these
    # modules will not specify "release" or "mock" entries. However, they do
    # need to have dict values so that the automatic port assignment will work.
    # The phases are (currently) aranged like this:
    # datafeed/raw/*: This is data that is straight from the microcontroller.
    # datafeed/sanitized/*: This is data that has been converted to a more
    #   sane format by the sanitation.py module.
    # datafeed/filtered/*: This is data that has been sent through a Kalman
    #   filter.
    # The format for the messages out of datafeed/sanitized/* should match
    # the format for the messages out of datafeed/filtered/*.
    "datafeed/raw/accelerometer": {
    },
    "datafeed/sanitized/accelerometer": {
        "listen": [
            "datafeed/raw/accelerometer"
        ]
    },
    "datafeed/filtered/accelerometer": {
    },
    "datafeed/raw/gyroscope": {
    },
    "datafeed/sanitized/gyroscope": {
        "listen": [
            "datafeed/raw/gyroscope"
        ]
    },
    "datafeed/filtered/gyroscope": {
    },
    "datafeed/raw/compass": {
    },
    "datafeed/sanitized/compass": {
        "listen": [
            "datafeed/raw/compass"
        ]
    },
    "datafeed/filtered/compass": {
    },
    "datafeed/raw/depth": {
        "mock": {
            "name": "mock.py",
            "args": ["--epoch", EPOCH, "--depth", 450]
        }
    },
    "datafeed/sanitized/depth": {
        "listen": [
            "datafeed/raw/depth"
        ]
    },
    "datafeed/filtered/depth": {
    },
    "datafeed/raw/battery_voltage": {
    },
    "datafeed/sanitized/battery_voltage": {
        "listen": [
            "datafeed/raw/battery_voltage"
        ]
    },
    "datafeed/filtered/battery_voltage": {
    },
}

# We need some extra lines for the debug channels. Aside from the logger,
# nothing should see debug messages.
debug_coms_to_add = []
for key, val in settings.items():
    if isinstance(val, dict):
        debug_coms_to_add.append(os.path.join(key, "debug"))

for debug_com in debug_coms_to_add:
    settings[debug_com] = {}

BASE_PORT = 20000
for key in sorted(settings.keys()):
    if isinstance(settings[key], dict):
        settings["util/logger"]["listen"].append(key)
        settings[key]["port"] = BASE_PORT
        BASE_PORT += 1

