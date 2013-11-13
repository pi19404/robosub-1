#!/usr/bin/env python

import sys
import os
import zmq
import time
import json
import numpy
import cv2
import cv2.cv as cv
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class VisionViewer(object):
    def __init__(self):
        #self.com = Communicator(
        #        module_name="util/vision_viewer",
        #        subscriber_buffer_length=81920,
        #        subscriber_high_water_mark=81920,
        #        settings_path="../../settings.json")
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.PAIR)
        self.socket.connect("tcp://127.0.0.1:50000")
        last_timestamp = 0.0
        while True:
            #missive = self.com.get_last_message('sensor/vision')
            #if missive and missive['timestamp'] > last_timestamp:
            #    last_timestamp = missive['timestamp']
            #    #msg = json.loads(missive['message'])
            #    print missive['message'][2]
            #    buf = buffer(missive['message'])
            #missive = self.com.get_last_image('sensor/vision')
            msg = self.socket.recv()
            self.socket.send(' ')
            if msg:
                buf = buffer(msg)
                im = numpy.frombuffer(buf, dtype=numpy.uint8)
                #im = im.reshape(480, 640, 3)
                im.shape = (480, 640, 3)
                cv2.imshow('test window', im)
                key = cv2.waitKey(1)
            else:
                time.sleep(.05)

if __name__ == '__main__':
    viewer = VisionViewer()
