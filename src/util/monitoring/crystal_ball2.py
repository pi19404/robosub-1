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
from time import sleep
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class VisionViewer(object):
    def __init__(self):
        self.com = Communicator(
                module_name="util/vision_viewer")
               # subscriber_buffer_length=81920,
               # subscriber_high_water_mark=81920)
        last_timestamp = 0.0
        self.com.connect_video_stream(50001, '192.168.1.3')
        #self.com.connect_video_stream(50001)
        print 'connected'
        while True:
            print 'hmm'
            try:
                im = self.com.recv_image()
            except ValueError:
                sleep(.1)
                im = None
            if im is not None:
                print 'got an image'
                b, g, r = cv2.split(im)
                cv2.imshow('test window', cv2.subtract(r, b))
                key = cv2.waitKey(1)

if __name__ == '__main__':
    viewer = VisionViewer()
