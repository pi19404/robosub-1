#!/usr/bin/env python

import sys
import os
import time
import json
import numpy
import cv2
import cv2.cv as cv
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class VisionViewer(object):
    def __init__(self):
        self.com = Communicator(
                module_name="util/vision_viewer",
                subscriber_buffer_length=8192,
                subscriber_high_water_mark=8192,
                settings_path="../../settings.json")
        last_timestamp = 0.0
        missive = {'timestamp': last_timestamp}
        while True:
            missive = self.com.get_last_message('sensor/vision')
            if missive and missive['timestamp'] > last_timestamp:
                last_timestamp = missive['timestamp']
                msg = json.loads(missive['message'])
                im = numpy.array(msg['im'], dtype=numpy.uint8)
                cv2.imshow('test window', im)
                key = cv2.waitKey(10)
            else:
                time.sleep(.05)

if __name__ == '__main__':
    viewer = VisionViewer()
