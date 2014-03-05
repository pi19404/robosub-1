#!/usr/bin/env python
import os
import cv2
import cv2.cv as cv
import zmq
#import threading
from multiprocessing.pool import ThreadPool
from time import strftime

#FIXME this logger should be JUST A LOGGER. Video grabbing should happen
#elsewhere.

class Streamer(object):

    def __init__(self, fp, settings):
        """Initialize video stream server.

        Args:

        """
        self._fp = fp
        self._settings = settings
        self._stream = {}
        self._stream['context'] = zmq.Context(1)
        self._stream['sockets'] = []
        self._stream['socket'] = \
                    self._stream['context'].socket(zmq.PAIR)
        self._stream['socket'].bind(
                    "tcp://*:{port}".format(port=self._settings['stream_port']))

    def worker(self, port):
        pass

    def process_image(self, packet):
        metadata = dict(dtype = str(self._fp.im.dtype), shape = self._fp.im.shape)
        try:
            self._stream['socket'].send_json(
                    metadata, flags=zmq.SNDMORE | zmq.NOBLOCK)
            self._stream['socket'].send(
                    self._fp.im, copy=True, track=False, flags=zmq.NOBLOCK)
        except zmq.ZMQError:
            pass


if __name__ == '__main__':
    pass
    #import sys
    #from Streamer import Streamer
    #sys.path.append(os.path.abspath('..'))
    #from frame_processor import FrameProcessor
    #from vision_settings import vision_settings
    #s = Streamer(FrameProcessor
