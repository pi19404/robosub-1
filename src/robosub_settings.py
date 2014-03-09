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
from sensor.vision.settings import settings as vision_settings
# TODO implement this when sensors are taken care of.
#from sensor.settings import settings as sensor_settings
from movement.settings import settings as movement_settings
from util.settings import settings as util_settings
from decision.settings import settings as decision_settings

# Maximum time a program might wait before checking for input.
EPOCH = "0.05"

settings = {
    "publisher_high_water_mark": 1024,
    "publisher_buffer_length": 1024,
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

# Merge other settings files into settings.
settings.update(vision_settings)
settings.update(movement_settings)
settings.update(util_settings)

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

