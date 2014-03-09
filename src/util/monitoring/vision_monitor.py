#!/usr/bin/env python

# FIXME not all of these are needed.
import sys
import os
import zmq
import time
import json
import numpy
import cv2
import cv2.cv as cv
import time
from time import sleep, time
from threading import Event
from multiprocessing.pool import ThreadPool
import numpy as np
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator
from robosub_settings import settings



class VisionViewer(object):
    def __init__(self, addr, camera):
        self._stream = {}
        self._stream['context'] = zmq.Context(1)
        self._stream['sockets'] = []
        self._module_name = 'sensor/vision/cam_{direction}'.format(direction=camera)
        for port_offset in range(settings[self._module_name]['port_span']):
            self._stream['sockets'] += \
                [self._stream['context'].socket(zmq.PAIR)]
            self._stream['sockets'][-1].connect(
                    "tcp://{addr}:{port}".format(
                    addr=addr, port=settings[self._module_name]['stream_port'] + port_offset))
        self._im_parts = [None] * settings[self._module_name]['port_span']
        self._im = None
        self._busy = Event()

        self._receiver()

    def _worker_receive(self, idx):
        metadata = self._stream['sockets'][idx].recv_json()
        message = self._stream['sockets'][idx].recv(
                    copy=True, track=False)
        buf = buffer(message)
        image = np.frombuffer(buf, dtype=metadata['dtype'])
        self._im_parts[idx] = image.reshape(metadata['shape'])

    def _receiver(self):
        p = ThreadPool(processes=settings[self._module_name]['port_span'])
        while True:
            print 'hi'
            start = time()
            p.map(func=self._worker_receive,
                    iterable=range(settings[self._module_name]['port_span']))
            print 'bye'
            try:
                self._im = np.vstack(self._im_parts)
                print time() - start
                cv2.imshow('vision_monitor', self._im)
                cv2.waitKey(1)
            except Exception:
                # Lazy
                pass
            self._busy.clear()

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

# FIXME actually use this.
def _command_line():
    parser = argparse.ArgmentParser(description="Vision Monitor")
    parser.add_argument('-i', '--ip', type=str,
            help='IP of computer hosting robosub video.')
    parser.add_argument('-c', '--camera', type=str,
            help='down, front, left, or right camera')


if __name__ == '__main__':
    viewer = VisionViewer('127.0.0.1', 'down')
