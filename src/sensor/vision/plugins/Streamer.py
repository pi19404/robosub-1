#!/usr/bin/env python
import os
import sys
#import cv2
#import cv2.cv as cv
import zmq
#import threading
import numpy as np
from threading import Thread, Event
from multiprocessing.pool import ThreadPool
from time import strftime
sys.path.append(os.path.abspath('../../..'))
from robosub_settings import settings

# TODO send video over multiple sockets to parallelize the unpacking on the other side.
# TODO split each image into smaller parts and send those over multiple
# TODO add a control port that listens for commands from the vision_monitor.

class Streamer(object):

    def __init__(self, fp, module_name):
        """Initialize video stream server.

        Args:

        """
        self._fp = fp
        self._module_name = module_name
        self._stream = {}
        self._stream['context'] = zmq.Context(1)
        self._stream['sockets'] = []
        for port_offset in range(settings[module_name]['port_span']):
            self._stream['sockets'] += \
                    [self._stream['context'].socket(zmq.PAIR)]
            self._stream['sockets'][-1].bind(
                    "tcp://*:{port}".format(
                    port=settings[module_name]['stream_port'] + port_offset))

        self._im_parts = None
        self._busy = Event()

        self._sender = Thread(target=self._sender)
        self._sender.daemon = True
        self._sender.start()


    def _sender(self):
        p = ThreadPool(processes=settings[self._module_name]['port_span'])
        while True:
            # Wait until we're told to get busy
            self._busy.wait()
            p.map(func=self._worker_send,
                    iterable=range(settings[self._module_name]['port_span']))
            self._busy.clear()

    def _worker_send(self, idx):
        metadata = dict(image_part=idx,
                dtype = str(self._fp.im.dtype),
                shape = self._im_parts[idx].shape)
        try:
            self._stream['sockets'][idx].send_json(
                    metadata, flags=zmq.SNDMORE | zmq.NOBLOCK)
            self._stream['sockets'][idx].send(
                    self._im_parts[idx], copy=True, track=False, flags=zmq.NOBLOCK)
        except zmq.ZMQError:
            pass

    def process_image(self, packet):
        # Don't even bother splitting and trying to send if we're already busy
        # with the previous image.
        if not self._busy.isSet():
            self._im_parts = np.split(
                    ary=self._fp.im,
                    indices_or_sections=settings[self._module_name]['port_span'])
            # Tell our sender thread to get busy!
            self._busy.set()

if __name__ == '__main__':
    pass
    #import sys
    #from Streamer import Streamer
    #sys.path.append(os.path.abspath('..'))
    #from frame_processor import FrameProcessor
    #from vision_settings import vision_settings
    #s = Streamer(FrameProcessor
