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
        "port": 20052,
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
        "port": 20053,
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/vision/fates": {
        "port": 20054,
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
        "port": 20055,
        "listen": [
            "decision",
            "decision/advisor",
            "movement/directive",
            "movement/stabilization",
            "movement/physical",
            "sensor/vision/cam_front",
            "sensor/vision/cam_down",
            "sensor/vision/fates",
            "sensor/accelerometer",
            "sensor/gyroscope",
            "sensor/compass",
            "sensor/depth",
            "sensor/battery_voltage",
            "util/logger"
        ],
        "release": {
            "name": "logger.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "logger.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/directive": {
        "port": 20056,
        "listen": [
            "decision",
            "sensor/vision/cam_front",
            "sensor/vision/cam_down"
        ],
        # The N values in each fuzzy set will define N + 1 regions.
        # The first number represents an x value whlie the second number
        # represents a y value. All points left of the lowest x value will have
        # the same y value. The regions between specified points will have
        # a value determined by connecting a straight line between those points.
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
        #        ------------------------\-/---------------------\---------------
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
            "name": "fuzzy_logic_fuzzifier.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/stabilization": {
        "port": 20057,
        "listen": [
            "movement/directive",
            "sensor/accelerometer"
        ],
        "release": {
            "name": "stabilization.py",
            "args": ["--epoch", EPOCH]},
        "mock": {
            "name": "stabilization.py",
            "args": ["--epoch", EPOCH]}
    },
    "movement/physical": {
        "port": 20058,
        "listen": [
            "decision/advisor",
            "movement/stabilization"
        ],
        "release": {"name": None},
        "mock": {"name": None}
    },
    # TODO: The commandline arguments are quite meaningful here, so they
    # should have something of a comment.
    "microcontroller": {
        "port": 20069,
        "release": {
            "name": "microcontroller_interface.py",
            "args": [
                "--epoch", "0.004",
                "--baudrate", "56818",
                "--port", '/dev/ttyUSB0',
                "--magnitude", "100"
        "mock": {
            "name": "microcontroller_interface.py",
            "args": [
                "--epoch", "0.004",
                "--debug",
                "--baudrate", "56818",
                "--magnitude", "100"
            ]}
    },
    "sensor/accelerometer": {
        "port": 20059,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/gyroscope": {
        "port": 20060,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/compass": {
        "port": 20061,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/serial/display_messages": {
        "port": 20062,
        "listen": [
            "sensor/serial/accelerometer",
            "sensor/serial/gyroscope",
            "sensor/serial/compass"
        ],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "decision": {
        "port": 20063,
        "listen": [
          "decision/advisor",
          "sensor/vision/cam_front",
          "sensor/vision/cam_down",
          "sensor/accelerometer",
          "sensor/gyroscope",
          "sensor/compass",
          "sensor/depth"
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
        "port": 20064,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "util/vision_viewer": {
        "port": 20065,
        "listen": [
            "sensor/vision/cam_front",
            "sensor/vision/cam_down"
        ],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/depth": {
        "port": 20066,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/battery_voltage": {
        "port": 20067,
        "listen": [],
        "release": {"name": None},
        "mock": {"name": None}
    },
    "sensor/sanitation": {
        "port": 20068,
        "listen": [
            "sensor/depth",
            "sensor/battery_voltage",
            "sensor/accelerometer",
            "sensor/gyroscope",
            "sensor/compass"
        ],
        "release": {"name": "sanitation.py"},
        "mock": {
            "name": "sanitation.py",
            "args": ["--debug"]}
    }
}

