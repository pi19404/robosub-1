#!/usr/bin/env python
# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""Creates and maintains Robosub vision processes.

Creates one process for every vision processor defined in
robosub/src/settings.json.

"""

import json # TODO remove this once the hack main function is removed
import sys
import os
import cv2
import cv2.cv as cv
import argparse
from multiprocessing import Process, Pipe
from stream_processor import StreamProcessor
from time import sleep
sys.path.append(os.path.abspath('../..'))
from util.communication.grapevine import Communicator

# TODO figure out how to initialize the camera driver settings through guvcview.

class Fates(object):
    """Create and maintain Robosub video logic processes."""
    def __init__(self, module_name, settings):
        """Create and maintain all video logic processes defined in settings.

        Args:
        settings -- dictionary of settings for Fates and processes managed
            by Fates.

        """
        self.module_name = module_name
        self.settings = settings # XXX consider making this a deepcopy?

        #self._vision_processors = []
        self._vision_processors = {}

        self._init_vision_processors()

        print 'size of vision pool is {size}'.format(
                    size=len(self._vision_processors))

        # Monitor vision processes and reinitialize any that fail.
        self._maintain_vision_processors()

    def _init_vision_processors(self):
        """Initialize process for each self.settings['vision_processors']."""
        for vp_name in self.settings[self.module_name]['vision_processors']:
            self._init_vision_process(vp_name)
            #print "adding vision process {vp}".format(vp=vp_name)
            #self._vision_processors[vp_name] = self._init_vision_process(vp_name)

    def _init_vision_process(self, process_name):
        """Initialize a process using settings given in vp_settings dict.

        Args:
        process_name: Name of process. Must match a process key entry in self.settings.

        """
        parent_conn, child_conn = Pipe()
        proc = Process(target = StreamProcessor,
                       name = process_name,
                       args = (process_name, self.settings, child_conn))
        #We want all managed processes to die if Fates dies.
        proc.daemon = True
        proc.start()
        parent_conn.send(1)
        self._vision_processors[process_name] = \
                {'process': proc, 'pipe': parent_conn}

    def _maintain_vision_processors(self):
        """Keep all processes in self._vision_processors alive.

        Every 'interval' seconds, check that all processes in
        self._vision_processors are responsive. Restart processes that are
        unresponsive or stopped.

        """
        #FIXME this is only checking that the processes are still running. Make
        #sure they are still responsive too. Pipe message passing?

        while True:
            sleep(self.settings[self.module_name]['maintenance_interval'])
            print 'doing another maintenance loop'
            for process_name in self._vision_processors.keys():
                self._maintain_proc(process_name)

    # TODO if a process has died or frozen, the process needs to be destroyed
    # and restarted after the camera has been given sufficient time to shut
    # down (~4sec?). This wait should be done in a separate thread so the rest
    # of the vision processing can continue.
    def _maintain_proc(self, process_name):
        proc = self._vision_processors[process_name]['process']
        parent_conn = self._vision_processors[process_name]['pipe']
        if not proc.is_alive() or not parent_conn.poll():
            # Process died unexpectedly or froze, restart it.
            self._init_vision_process(process_name)
        else:
            # Send the token to the child process. If the child doesn't freeze
            # we should get it back before the next maintenance check.
            parent_conn.send(parent_conn.recv())

def _main(args):
    """Start the vision daemon."""
    settings = json.loads(open(args.settings_path, 'r').read())
    if args.video:
        settings['sensor/vision/visiond']['vision_processors'] = ['sensor/vision/cam_fake']
        settings['sensor/vision/cam_fake']['recorded_video'] = args.video
    if args.plugins:
        settings['sensor/vision/cam_fake']['plugins'] = args.plugins
    v = Fates('sensor/vision/visiond', settings)

def _command_line():
    """Parse command line arguments."""
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-s', '--settings_path', type=str,
            default='./settings.json',
            help='Settings file path.')
    parser.add_argument('-v', '--video', type=str,
            help='Video file or stream to open.')
    # TODO figure out how to list plugins?
    parser.add_argument('-p', '--plugins', type=str, nargs='+',
            help='Plugins to load.')
    return parser.parse_args()

#TODO delete this
if __name__ == '__main__':
    _main(_command_line())
