# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""Creates and maintains Robosub vision processes.

Creates one process for every vision processor defined in
robosub/src/settings.json.

"""

import json #TODO remove this once the hack main function is removed
import sys
import os
import cv2
import cv2.cv as cv
from multiprocessing import Process
from VisionProcessor import VisionProcessor
from logging import VideoLogger
from time import sleep
sys.path.append(os.path.abspath('../..'))
from util.communication.grapevine import Communicator

#TODO figure out how to force a camera to have a certain index.
#TODO figure out how to initialize the camera driver settings through guvcview.

class Fates(object):
    """Create and maintain Robosub video logic processes."""
    def __init__(self, module_name, settings):
        """Create and maintain all video logic processes defined in settings.

        Args:
        settings -- dictionary of settings for Fates and processes managed
            by Fates.

        """
        self.module_name = module_name
        self.settings = settings #XXX consider making this a deepcopy?

        #All vision logic processes go here.
        self._vision_pool = []

        #Start all vision processes.
        self._init_vision_pool()

        print 'size of vision pool is {size}'.format(size=len(self._vision_pool))

        #Monitor vision processes and reinitialize any that fail.
        self._maintain_vision_pool(
                    settings[self.module_name]['maintenance_interval'])

    def _init_vision_pool(self):
        """Initialize process for each self.settings['vision_processors']."""
        for vp_name in self.settings[self.module_name]['vision_processors']:
            self._vision_pool += [self._init_vision_process(vp_name)]

    def _init_vision_process(self, name):
        """Initialize a process using settings given in vp_settings dict.

        Args:
        name: Name of process. Must match a process key entry in self.settings.

        """
        proc = Process(target = VisionProcessor,
                       name = name,
                       args = (name, self.settings))
        #We want all managed processes to die if Fates dies.
        proc.daemon = True
        proc.start()
        return proc

    def _maintain_vision_pool(self, interval=5):
        """Keep all processes in self._vision_pool alive.

        Every 'interval' seconds, check that all processes in self._vision_pool
        are responsive. Restart processes that are unresponsive or stopped.

        Args:
        interval -- How often in seconds to check processes in the 
            self._vision_pool list for responsiveness. (default 5)

        """
        #FIXME this is only checking that the processes are still running. Make
        #sure they are still responsive too. Pipe message passing?
        while True:
            sleep(interval)
            print 'doing another maintenance loop'
            for idx, proc in enumerate(self._vision_pool):
                if not proc.is_alive():
                    #Process died unexpectedly, restart it.
                    self._vision_pool[idx] = self._init_vision_process(
                                    self.settings[self.module_name]['vision_processors'][idx])


#TODO delete this
if __name__ == '__main__':
    with open('camera_settings.json') as settings_file:
        vision_settings = json.loads(settings_file.read())
        f = Fates('sensor/vision/Fates',vision_settings)
