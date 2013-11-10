import json #TODO remove this once the hack main function is removed
import cv2
import cv2.cv as cv
from multiprocessing import Pool, Process
from VisionProcessor import VisionProcessor
from logging import VideoLogger
from time import sleep

#TODO figure out how to force a camera to have a certain index.
#TODO figure out how to initialize the camera driver settings through guvcview.

class Fates(object):
    '''Create and maintain Robosub video logic processes.'''

    def __init__(self, settings):
        '''Create and maintain all video logic processes defined in settings.

        Args:
        settings -- dictionary of settings for Fates and processes managed
            by Fates.

        '''
        self.settings = settings #XXX consider making this a deepcopy?

        #All vision logic processes go here.
        self._vision_pool = []
        #Start all vision processes.
        self._init_vision_pool()

        #The vision processes are running. Now maintain them.
        self._maintain_vision_pool(settings['maintenance_interval'])

    def _init_vision_pool(self):
        '''Initialize process for each self.settings['vision_processors'].'''
        for vp_settings in self.settings['vision_processors']:
            self._vision_pool += [self._make_process(vp_settings)]

    def _make_process(self, vp_settings):
        '''Initialize a process using settings given in vp_settings dict.

        Args:
        vp_settings: Dict of settings for one Vision processor object.

        '''
        proc = Process(target = VisionProcessor,
                       name = vp_settings['camera'] + '_cam',
                       args = (vp_settings,))
        #We want all managed processes to die if Fates dies.
        proc.daemon = True
        proc.start()
        return proc

    def _maintain_vision_pool(self, interval=5):
        '''Keep all processes in self._vision_pool alive.

        Every 'interval' seconds, check that all processes in self._vision_pool
        are responsive. Restart processes that are unresponsive or stopped.

        Args:
        interval -- How often in seconds to check processes in the 
            self._vision_pool list for responsiveness. (default 5)

        '''
        #FIXME this is only checking that the processes are still running. Make
        #sure they are still responsive too. Pipe message passing?
        while True:
            sleep(interval)
            print 'doing another maintenance loop'
            for idx, proc in enumerate(self._vision_pool):
                if not proc.is_alive():
                    #Process died unexpectedly, restart it.
                    self._vision_pool[idx] = self._make_process(
                                    self.settings['vision_processors'][idx])


#TODO delete this
if __name__ == '__main__':
    with open('camera_settings.json') as settings_file:
        vision_settings = json.loads(settings_file.read())
        f = Fates(vision_settings)
