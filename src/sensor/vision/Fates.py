import json #TODO remove this once the hack main function is removed
import cv2
import cv2.cv as cv
from multiprocessing import Pool, Process
from VisionProcessor import VisionProcessor
from logging import VideoLogger
from time import sleep

#TODO figure out how to force a camera to have a certain index.
#TODO figure out how to initialize the camera driver settings through guvcview.

################################################################################
# @class Fates
# @brief:  This class initializes and maintains the video logic processes for
#           a robosub. Settings for this object are provided
################################################################################
class Fates(object):
    ############################################################################
    # @method __init__()
    # @brief:  Create VisionProcessor objects for all Robosub cameras and
    #       restart the processes if they ever fail.
    # @param self: Standard python object reference.
    # @param settings:  Dictionary of settings for all Robosub vision logic.
    ############################################################################
    def __init__(self, settings):
        self.settings = settings #XXX consider making this a deepcopy?

        #All vision logic processes go here.
        self._vision_pool = []
        #Start all 
        self._init_vision_pool()

        #The vision processes are running. Now maintain them.
        self._maintain_vision_pool(settings['maintenance_interval'])

    ############################################################################
    # @method _init_vision_pool()
    # @brief:  Initializes a process for a VisionProcessor object for each
    #       entry in the self.settings['cameras'] list, and stores the process
    #       in the self._vision_pool list.
    # @param self: Standard python object reference.
    ############################################################################
    def _init_vision_pool(self):
        for camera_settings in self.settings['cameras']:
            self._vision_pool += [self._make_process(camera_settings)]

    ############################################################################
    # @method _make_process()
    # @brief:  Create a a process for a VisionProcess object, starts it, and
    #           returns it.
    # @param self: Standard python object reference.
    # @param camera_settings: Dictionary of settings for a single
    #           VisionProcessor object.
    ############################################################################
    def _make_process(self, camera_settings):
        proc = Process(target = VisionProcessor,
                       name = camera_settings['camera'] + '_cam',
                       args = (camera_settings,))
        proc.daemon = True
        proc.start()
        return proc

    ############################################################################
    # @method _maintain_vision_pool()
    # @brief:  Periodically makes sure all VideoProcessor object in the vision
    #           pool are still functioning. Restarts any that are not.
    # @param self: Standard python object reference.
    # @param interval:
    ############################################################################
    def _maintain_vision_pool(self, interval=10):
        while True:
            sleep(interval)
            print 'doing another maintenance loop'
            for idx, proc in enumerate(self._vision_pool):
                #FIXME Does this fail if process is frozen? This might only work
                #if the thread crashes. Consider enhancing this with a
                #query/response pipe. Figure out how a real maintenance daemon
                #does it.
                if not proc.is_alive():
                    #Process died unexpectedly, restart it.
                    self._vision_pool[idx] = \
                            self._make_process(self.settings['cameras'][idx])


#TODO delete this
if __name__ == '__main__':
    with open('camera_settings.json') as settings_file:
        vision_settings = json.loads(settings_file.read())
        f = Fates(vision_settings)
