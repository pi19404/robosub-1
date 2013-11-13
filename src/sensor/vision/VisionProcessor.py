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
    def __init__(self, settings):
        """Initialize video capture, logger, and processing plugins.

        Args:
        settings -- dict of settings for this object.

        """
        self.settings = settings
        #self.cap = cv2.VideoCapture(settings['dev'])
        self.cap = cv2.VideoCapture(0)#FIXME this line is hacked in
        self.com = Communicator(
                    module_name='sensor/vision',
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
        self.cap.set(cv.CV_CAP_PROP_FOURCC,
                    cv.CV_FOURCC(*list(settings['codec'].encode('ascii'))))
        self.cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, settings['height'])
        self.cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, settings['width'])
        self._vision_process()

    def _vision_process(self):
        print self.settings['camera'] + '_heck_yeah'
        context = zmq.Context()
        socket = context.socket(zmq.PAIR)
        socket.bind("tcp://127.0.0.1:50000")
        while True:
            #TODO add some error checking before calling modules. They may
            #be crashed. Reinstantiate them before calling them if they do.
            #TODO make it so this loop happens once every 'settings['fps']
            #seconds. (endtime - starttime == whatever needed process
            #correct fps.
            got_image, im = self.cap.read()
            if got_image:
                #self.com.publish_image(im)
                socket.send(im)
                socket.recv()
                sleep(.2)


