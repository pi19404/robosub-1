#!/usr/bin/env python
import os
import cv2
import cv2.cv as cv
import threading
from time import strftime

#FIXME this logger should be JUST A LOGGER. Video grabbing should happen
#elsewhere.

class VideoLogger(object):
    #class variables
    #XXX does this filepath handling conform with Robosub standards?
    DIR = os.path.dirname(os.path.abspath(__file__)) + '/'
    FOURCC = cv.CV_FOURCC(*list('MJPG'))

    ############################################################################
    # @method __init__()
    #FIXME this is inaccurate. Update it.
    # @brief:  Initialize opencv capture and writer objects.
    # @param self:  standard python object reference.
    # @param vid_src:  Camera, video file, or stream to read via opencv.
    # @param vid_out:  Location to write destination video.
    # @param width:  Width to set video src and dest.
    # @param height:  Height to set video src and dest.
    # @param fps:  Frames per second ot set video output.
    ############################################################################
    def __init__(self, settings):
        self.settings = settings
        self.writers = {}

        self.videoWriter = cv2.VideoWriter()
        #FIXME generate a proper output filename
        self.cap = None
        self.image = None
        self.capture_lock = threading.RLock()
        self.capturing = threading.Event()

    def _init_cap(self):
        self.cap = cv2.VideoCapture(self.vid_src)
        self.cap.set(cv.CV_CAP_PROP_FOURCC, self.FOURCC)
        self.cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, self.height)
        self.cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, self.width)

    def _init_writer(self, key):
        dest = self.DIR + strftime('%yy_%mm_%dd_%Hh_%Mm_%Ss_') + key + '.avi'
        self.writers[key] = cv2.VideoWriter()
        self.writers[key].open(
                    filename = dest,
                    fourcc = self.FOURCC,
                    fps = settings['fps'],
                    frameSize = (settings['width'], settings['height']))

    def _destroy_cap(self):
        #FIXME change this to destroy only windows we create.
        #cv2.destroyAllWindows()
        cv2.destroyWindow('video test')
        self.cap.release()
        self.cap = None

    ############################################################################
    # @method start()
    # @brief:  starts the video logging thread.
    # @param self:  standard python object reference.
    ############################################################################
    def start(self):
        self._init_cap()
        if 'raw' in self.settings['log']:
            self._init_writer('raw')
        with self.capture_lock:
            t = threading.Thread(target=self._start)
            t.setDaemon(True)
            self.capturing.set()
            t.start()

    #FIXME by stopping this, we make it impossible to start again correctly.
    #fix this so start/stop can be toggled.
    def stop(self):
        self.capturing.clear()
        self._destroy_cap()

    ############################################################################
    # @method _write_image()
    # @brief:  Write self.image to writer file.
    # @param self:  standard python object reference.
    ############################################################################
    def _write_image(self, writer, image=None):
        if image is None:
            self.writers.write(self.image)
        else:
            self.writers.write(image)

    def _start(self):
        while self.capturing.is_set():
            _, self.image = self.cap.read()
            self._write_image('raw')
            #cv2.imshow('video test', self.image)
            key = cv2.waitKey(10)
            if key == 27:
                self.stop()

    def __del__(self):
        cv2.destroyAllWindows()
