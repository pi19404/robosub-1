import json
from logging import video_logger

class VideoIOError(Exception):
    pass

class Fates(object):
    def __init__(self):
        with open('camera_settings.json') as settings_file:
            settings = json.loads(settings_file.read())
        for camera in settings['cameras']:
            print camera
