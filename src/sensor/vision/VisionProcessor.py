import cv2
import cv2.cv as cv
from logging import VideoLogger
from time import sleep
import sys
import os
import json
import zmq
from importlib import import_module
sys.path.append(os.path.abspath('../..'))
from util.communication.grapevine import Communicator


class VisionProcessor(object):
    """Open a video stream and process frames. Report finds to the grapevine."""
    def __init__(self, module_name, settings):
        """Initialize video capture, logger, and processing plugins.

        Args:
        settings -- dict of settings for this object.

        """
        self.module_name = module_name
        self.settings = settings[module_name]
        #self.cap = cv2.VideoCapture(settings['dev'])
        # A symlink should have been created based on the serial number for
        # each camera in use. It'll help us find the correct video device
        # index.
        # FIXME error check the path. If the camera doesn't exist, neither will
        # the symlink.
        self.cap = cv2.VideoCapture(int(os.path.realpath(
                    self.settings['symlink'])[-1]))
        self.com = Communicator(
                    module_name=module_name,
                    subscriber_buffer_length=81920,
                    subscriber_high_water_mark=81920)

        #Images from self.cap will be processed by the modules stored here.
        self._vision_modules = []
        self.logger = VideoLogger.VideoLogger(settings = settings)

        #Load, instantiate, and store the modules defined in the settings file.
        for vp in self.settings['plugins']:
            #Name of module and the class should be the same.
            module_obj = getattr(import_module(vp), vp)()
            self._vision_modules += [module_obj]

        #XXX figure out if setting any of these is really necessary.
        #Preconfiguring the driver should take care of these.

        #Configure the VideoCapture object.
        #FIXME fix the JSON load so it doesn't make all strings unicode.
        self.cap.set(cv.CV_CAP_PROP_FOURCC, cv.CV_FOURCC(
                    *list(self.settings['codec'].encode('ascii'))))
        self.cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, self.settings['height'])
        self.cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, self.settings['width'])
        self._vision_process()

    def _vision_process(self):
        print self.settings['symlink'] + '_heck_yeah'
        self.com.bind_video_stream(self.settings['stream_port'])
        while True:
            #TODO add some error checking before calling modules. They may
            #be crashed. Reinstantiate them before calling them if they do.
            #TODO make it so this loop happens once every 'settings['fps']
            #seconds. (endtime - starttime == whatever needed process
            #correct fps.
            # FIXME an unplugged or suddenly-dead camera will continue to
            # return True and the last valid image. We need to find a way to
            # detect that so that we can generate an error.
            got_image, im = self.cap.read()
            if got_image:
                #self.com.publish_image(im)
                self.com.send_image(im)
                sleep(.1)
            else:
                raise Exception


