#!/usr/bin/env python

import sys
import os
import zmq
import time
import json
import numpy
import cv2
import cv2.cv as cv
import time
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class VisionViewer(object):
    def __init__(self):
        self.com = Communicator(
                module_name="util/vision_viewer",
                subscriber_buffer_length=81920,
                subscriber_high_water_mark=81920,
                settings_path="../../settings.json")
        last_timestamp = 0.0
        self.com.connect_video_stream(50000)
        print 'connected'
        while True:
            try:
                im = self.com.recv_image()
            except ValueError:
                im = None
            if im is not None:
                print 'got an image'
                cv2.imshow('test window', im)
                key = cv2.waitKey(1)

if __name__ == '__main__':
    viewer = VisionViewer()
