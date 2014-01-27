#!/usr/bin/env python
import cv2
import cv2.cv as cv
from logging.video_logger import VideoLogger
from time import sleep, time
import sys
import os
import json
import zmq
from multiprocessing.pool import ThreadPool
from threading import Thread
from importlib import import_module
from frame_preprocessor import FramePreprocessor
sys.path.append(os.path.abspath('../..'))
from util.communication.grapevine import Communicator


class StreamProcessor(object):
    """Open a video stream and process frames. Report finds to the grapevine."""
    def __init__(self, module_name, settings, pipe):
        """Initialize video capture, logger, and processing plugins.

        Args:
        module_name -- TODO
        settings -- dict of settings for this object.
        pipe -- TODO

        """
        self.module_name = module_name
        self.master_settings = settings['sensor/vision/visiond']
        self.settings = settings[module_name]
        self.preprocessor = FramePreprocessor()
        # Nagging messages from the parent come from here. The process must
        # respond quickly enough or be killed by the parent.
        self._pipe = pipe

        self._got_im, self._im = None, None
        # A symlink should have been created based on the serial number for
        # each camera in use. It'll help us find the correct video device
        # index.
        # FIXME error check the path. If the camera doesn't exist, neither will
        # the symlink.
        # TODO enable using videos or images as a source.
        self._init_stream()

        # Individual stream images will be processed by modules stored here.
        self._plugins = []

        # Load, instantiate, and store the modules defined in the settings file.
        for vp in self.settings['plugins']:
            # Name of module and the class should be the same.
            module_obj = getattr(import_module(
                    '..'+vp, package='plugins.subpkg'), vp)()
            self._plugins += [module_obj]

        #Configure the VideoCapture object.
        self._vision_process()

    def _init_stream(self):
        if self.settings['stream_type'] == 'device':
            cam_path = os.path.realpath(self.settings['symlink'])
            # Check that the symlink points to something.
            if '/dev/video' not in cam_path:
                raise Exception('Broken symlink {sym}'.format(
                        symlink=self.settings['symlink']))
            else:
                self._cap = cv2.VideoCapture(int(cam_path[-1]))
                self._cap.set(cv.CV_CAP_PROP_FOURCC, cv.CV_FOURCC(
                        *list(self.settings['codec'].encode('ascii'))))
                self._cap.set(cv.CV_CAP_PROP_FPS, self.settings['fps'])
                self._cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, self.settings['height'])
                self._cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, self.settings['width'])
            self._frame_grabber_thread = Thread(target=self.refresh_frame)
            self._frame_grabber_thread.daemon = True
            self._frame_grabber_thread.start()

        elif self.settings['stream_type'] == 'recorded_video':
            # The 'symlink' key didn't exist. This must be a test with recorded
            # video.
            self._cap = cv2.VideoCapture(self.settings['recorded_video'])
            self._frame_grabber_thread = None
        else:
            raise Exception(
                    'No stream type specified for {module_name}'.format(
                    module_name=module_name))

    def _get_frame(self):
        if self._frame_grabber_thread is not None:
            return (self._got_im, self._im)
        else:
            return self._cap.read()

    def refresh_frame(self):
        failed_count = 0
        # Assuming 30fps, 150 fails in a row means 5s of no video.
        while failed_count < self.master_settings['max_failed_frames']:
            self._got_im, self._im = self._cap.read()
            if not self._got_im:
                failed_count += 1
            else:
                failed_count = 0
        raise Exception('Camera sucks')

    def _vision_process(self):
        #self._com.bind_video_stream(self.settings['stream_port'])
        self._got_im, self._im = None, None


        while True:
            start_time = time()
            # TODO make it so this loop happens once every 'settings['fps']
            # seconds. (endtime - starttime == whatever needed process
            # correct fps.
            # FIXME an unplugged or suddenly-dead camera will continue to
            # return True and the last valid image. We need to find a way to
            # detect that so that we can generate an error.
            #got_im, im = self._cap.read()
            got_im, im = self._get_frame()
            if got_im:
                # Check if parent sent a message over the pipe.
                if self._pipe.poll() is True:
                    # Send the message back to parent to prove this process
                    # isn't frozen.
                    self._pipe.send(self._pipe.recv())

                #self._com.send_image(im)
                self.preprocessor.preprocess(im)
                for plugin in self._plugins:
                    retval, new_im = plugin.process_image(im)
                    if retval is not None:
                        self._com.publish_message(retval)
            try:
                # Try to sleep until we took the correct amount of time.
                sleep((start_time + (1.0 / self.settings['fps'])) - time())
            except IOError:
                print 'took too much time'
                pass
