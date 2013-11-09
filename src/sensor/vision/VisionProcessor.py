import cv2
import cv2.cv as cv
from logging import VideoLogger
from time import sleep

class VisionProcessor(object):
    def __init__(self, settings):
        print settings['camera']
        self._vision_functions = []
        self.settings = settings
        self.cap = cv2.VideoCapture(settings['dev'])
        self.logger = VideoLogger.VideoLogger(settings = settings)

        self.cap.set(cv.CV_CAP_PROP_FOURCC,
                    cv.CV_FOURCC(*list(settings['codec'].encode('ascii'))))
        self.cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, settings['height'])
        self.cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, settings['width'])
        self._vision_process()

    def _vision_process(self):
        print self.settings['camera'] + '_heck_yeah'
        while True:
            sleep(100)
