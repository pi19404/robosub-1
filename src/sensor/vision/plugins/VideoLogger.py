#!/usr/bin/env python
import os
import sys
import cv2
import cv2.cv as cv
#import threading
from multiprocessing.pool import ThreadPool
from time import strftime
sys.path.append(os.path.abspath('../../..'))
from robosub_settings import settings

#FIXME this logger should be JUST A LOGGER. Video grabbing should happen
#elsewhere.

class VideoLogger(object):
    #class variables
    #XXX does this filepath handling conform with Robosub standards?
    DIR = os.path.dirname(os.path.abspath(__file__)) + '/'

    def __init__(self, fp, module_name):
        """Initialize opencv capture and writer objects.

        Args:

        """
        self._settings = settings[module_name]
        self._writer = None
        self._pool = ThreadPool(1)
        self._fp = fp
        self._init_writer()

    def _init_writer(self):
        #dest = self.DIR + strftime('%yy_%mm_%dd_%Hh_%Mm_%Ss_') + key + '.avi'
        print self._settings
        dest = '{dir}{timestamp}{name}.avi'.format(
                dir=self.DIR,
                timestamp=strftime('%yy_%mm_%dd_%Hh_%Mm_%Ss_'),
                name=self._settings['name'])
        self._writer = cv2.VideoWriter()
        self._writer.open(
                    filename = dest,
                    fourcc = cv.CV_FOURCC(*list('MJPG')),
                    fps = 5,
                    frameSize = (640, 480))

    ############################################################################
    # @method _write_image()
    # @brief:  Write self.image to writer file.
    # @param self:  standard python object reference.
    ############################################################################
    def _write_image(self, _writer, im):
        try:
            self._writer.write(im)
        except Exception:
            self._init_writer()
            try:
                self._writer.write(im)
            except:
                raise Exception("Could not write image")

    def process_image(self, packet):
        self._writer.write(self._fp.im)
        #self._pool.apply_async(func=self._write_image, args=(self._fp.im,))

