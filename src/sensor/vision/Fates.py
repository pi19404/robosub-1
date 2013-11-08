import json
import cv2
import cv2.cv as cv
import threading
from logging import VideoLogger

class VideoIOError(Exception):
    pass

class Fates(object):
    def __init__(self):
        self.cameras = {}

        with open('camera_settings.json') as settings_file:
            settings = json.loads(settings_file.read())
            settings['fourcc'] = cv.CV_FOURCC(*list(settings['codec']))
            for camera in settings['cameras']:
                if camera['enable']:
                    self._init_cap(camera)

    def _init_cap(self, camera):
        fourcc = cv.CV_FOURCC(*list(settings['codec']))
        cap = cv2.VideoCapture(camera['dev'])
        cap.set(cv.CV_CAP_PROP_FOURCC, fourcc)
        cap.set(cv.CV_CAP_PROP_FRAME_HEIGHT, camera['height'])
        cap.set(cv.CV_CAP_PROP_FRAME_WIDTH, camera['width'])
        self.cap[camera['camera']]['logger'] =
                    VideoLogger.VideoLogger(settings = camera)
        self.cap[camera['camera']]['cap'] = 
                    cv2.VideoCapture(camera['dev'])

