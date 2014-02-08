#!/usr/bin/env python
import os
import cv2
import cv2.cv as cv
#import threading
from multiprocessing.pool import ThreadPool
from time import strftime

#FIXME this logger should be JUST A LOGGER. Video grabbing should happen
#elsewhere.

class VideoLogger(object):
    #class variables
    #XXX does this filepath handling conform with Robosub standards?
    DIR = os.path.dirname(os.path.abspath(__file__)) + '/'

    def __init__(self, fp):
        """Initialize opencv capture and writer objects.

        Args:

        """
        self._writer = None
        self._pool = ThreadPool(1)
        self._fp = fp

    def _init_writer(self, im):
        dest = self.DIR + strftime('%yy_%mm_%dd_%Hh_%Mm_%Ss_') + key + '.avi'
        self._writer = cv2.VideoWriter()
        self.writer.open(
                    filename = dest,
                    fourcc = cv.CV_FOURCC(*list('MJPG')),
                    fps = 5,
                    frameSize = (im.shape[0], im.shape[1]))

    ############################################################################
    # @method _write_image()
    # @brief:  Write self.image to writer file.
    # @param self:  standard python object reference.
    ############################################################################
    def _write_image(self, writer, im):
        try:
            self.writer.write(im)
        except AttributeError:
            self._init_writer(im)
            try:
                self.writer.write(im)
            except:
                raise Exception("Could not write image")

    def process_image(self, packet):
        self._pool.apply_async(func=self._write_image, args=(self._fp.im,))

