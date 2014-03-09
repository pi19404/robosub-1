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
from threading import Event
from multiprocessing.pool import ThreadPool
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
        self._split_im = [None] * settings[self._module_name]['port_span']
        self._im = None
        self._busy = Event()

        self._receiver()
        #self._receiver = Thread(target=self._receiver)
        #self._receiver.daemon = True
        #self._receiver.start
    #def recv_image(self):
    #    """Receive an image from the connected stream client socket."""
    #    metadata = self.subscribers['stream']['socket'].recv_json()
    #    message = self.subscribers['stream']['socket'].recv(
    #                copy=True, track=False)
    #    buf = buffer(message)
    #    image = frombuffer(buf, dtype=metadata['dtype'])
    #    return image.reshape(metadata['shape'])

    def _worker_receive(self, idx):
        metadata = self._stream['sockets'][idx].recv_json()
        message = self._stream['sockets'].recv(
                    copy=True, track=False)
        buf = buffer(message)
        image = frombuffer(buf, dtype=metadata['dtype'])
        self._split_im[idx] = image.reshape(metadata['shape'])

    def _receiver(self):
        p = ThreadPool(processes=settings[self._module_name]['port_span'])
        while True:
            self._busy.wait()
            p.map(func=self._worker_receive,
                    iterable=range(settings[self._module_name]['port_span']))
            try:
                self._im = np.hstack(self._split_im)
                print self._im.shape
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

    #def connect_video(self, port, addr='127.0.0.1'):
    #    """Connect one end of a socket pair for video streaming."""
    #    self.subscribers['stream'] = {}
    #    self.subscribers['stream']['context'] = zmq.Context(1)
    #    self.subscribers['stream']['socket'] = \
    #                self.subscribers['stream']['context'].socket(zmq.PAIR)
    #    self.subscribers['stream']['socket'].connect(
    #                "tcp://{addr}:{port}".format(addr=addr, port=port))

def _command_line():
    parser = argparse.ArgmentParser(description="Vision Monitor")
    parser.add_argument('-i', '--ip', type=str,
            help='IP of computer hosting robosub video.')
    parser.add_argument('-c', '--camera', type=str,
            help='down, front, left, or right camera')


if __name__ == '__main__':
    viewer = VisionViewer('127.0.0.1', 'down')
