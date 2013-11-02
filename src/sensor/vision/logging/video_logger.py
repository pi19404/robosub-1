#!/usr/bin/env python
import cv2
import cv2.cv as cv
import threading

#FIXME this logger should be JUST A LOGGER. Video grabbing should happen
#elsewhere.

class VideoLogger(object):
    #class variables
    FOURCC_MJPEG = cv.CV_FOURCC('M', 'J', 'P', 'G')

    ############################################################################
    # @method __init__()
    # @brief:  Initialize opencv capture and writer objects.
    # @param self:  standard python object reference.
    # @param vidSrc:  Camera, video file, or stream to read via opencv.
    # @param vidDestPrefix:  Prefix of filenames to write destination video.
    # @param width:  Width to set video src and dest.
    # @param height:  Height to set video src and dest.
    # @param fps:  Frames per second ot set video output.
    ############################################################################
    def __init__(self, vidSrc, vidDestPrefix, width, height, fps):
        self.vidSrc = vidSrc
        self.videoWriter = cv2.VideoWriter()

        #FIXME generate a proper output filename
        self.videoWriter.open(
                    filename = str(vidDestPrefix) + str(vidSrc) + '.avi',
                    fourcc = self.FOURCC_MJPEG,
                    fps = fps,
                    frameSize = (width, height))
        self.cap = cv2.VideoCapture(vidSrc)
        self.cap.set(cv.CV_CAP_PROP_FOURCC, self.FOURCC_MJPEG)
        self.cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, height)
        self.cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, width)
        self.image = None
        self.capture_lock = threading.RLock()
        self.capturing = threading.Event()

    ############################################################################
    # @method start()
    # @brief:  starts the video logging thread.
    # @param self:  standard python object reference.
    ############################################################################
    def start(self):
        with self.capture_lock:
            t = threading.Thread(target=self._start)
            t.setDaemon(True)
            self.capturing.set()
            t.start()

    #FIXME by stopping this, we make it impossible to start again correctly.
    #fix this so start/stop can be toggled.
    def stop(self):
        self.capturing.clear()
        self.cap.release()

    ############################################################################
    # @method _getImage()
    # @brief:  Grab an image from the video source and set it to self.image
    # @param self:  standard python object reference.
    ############################################################################
    def _getImage(self):
        _, self.image = self.cap.read()

    ############################################################################
    # @method _writeImage()
    # @brief:  Write self.image to writer file.
    # @param self:  standard python object reference.
    ############################################################################
    def _writeImage(self):
        # TODO: make this safer so we arent trying to write a bogus image.
        self.videoWriter.write(self.image)

    def _start(self):
        while self.capturing.is_set():
            self._getImage()
            self._writeImage()
            #cv2.imshow('video test', self.image)
            key = cv2.waitKey(10)
            if key == 27:
                break	

    #def __del__(self):
    #    self.cap.release()
