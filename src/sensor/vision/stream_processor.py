#!/usr/bin/env python
import cv2
import cv2.cv as cv
from time import sleep, time
import sys
import os
#from multiprocessing.pool import ThreadPool
from threading import Thread, Event
from importlib import import_module
from frame_processor import FrameProcessor
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
        self.master_settings = settings['sensor/vision/control']
        self.settings = settings[module_name]
        self.processor = FrameProcessor()
        self._com = Communicator(module_name)
        # Nagging messages from the parent come from here. The process must
        # respond quickly enough or be killed by the parent.
        self._pipe = pipe
        self._fresh_frame_available = Event()

        self._got_im, self._im = None, None
        self._init_stream()

        # Individual stream images will be processed by modules stored here.
        self._plugins = []

        # Load, instantiate, and store the modules defined in the settings file.
        for plugin_name in self.settings['plugins']:
            # Name of module and the class should be the same.
            module_obj = getattr(import_module(
                    '..'+plugin_name, package='plugins.subpkg'),
                    plugin_name)(self.processor, self.settings)
            self._plugins += [module_obj]

        # Configure the VideoCapture object.
        self._vision_process()

    def _init_stream(self):
        """Initialize the video stream."""

        if self.settings['stream_type'] == 'device':
            cam_path = os.path.realpath(self.settings['symlink'])
            # Check that the symlink points to something that really exists.
            if '/dev/video' not in cam_path:
                raise Exception('Broken symlink {sym}'.format(
                        sym=self.settings['symlink']))
            self._cap = cv2.VideoCapture(int(cam_path[-1]))
            self._cap.set(cv.CV_CAP_PROP_FPS, self.settings['fps'])
            self._frame_grabber_thread = Thread(target=self.refresh_frame)
            self._frame_grabber_thread.daemon = True
            self._frame_grabber_thread.start()
            #self._frame_grabber_thread = None

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
        """Get the most recent frame from the video stream."""

        if self._frame_grabber_thread is not None:
            self._fresh_frame_available.wait()
            return (self._got_im, self._im)
        else:
            return self._cap.read()

    def refresh_frame(self):
        """Get and store latest frame from video stream as fast as possible."""
        failed_count = 0
        # Assuming 30fps, 150 fails in a row means 5s of no video.
        while failed_count < self.master_settings['max_failed_frames']:
            self._got_im, self._im = self._cap.read()
            self._fresh_frame_available.set()
            self._fresh_frame_available.clear()
            if not self._got_im:
                failed_count += 1
            else:
                failed_count = 0
        raise Exception('Camera sucks')

    def _vision_process(self):
        """Detect obstacle details from video stream. Report findings."""

        while True:
            start_time = time()
            got_im, im = self._get_frame()
            #print 'new frame'
            if self._pipe.poll() is True:
                # Send the message back to parent to prove this process
                # isn't frozen.
                self._pipe.send(self._pipe.recv())
            if got_im:
                # Check if parent sent a message over the pipe.

                self.processor.load_im(im)
                # self.processor.hacky_display() # FIXME this needs to go away.
                packet = {}
                for plugin in self._plugins:
                    plugin.process_image(packet)
                self._com.publish_message(packet)
