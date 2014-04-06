# line follow task

import os
import sys
import time
import numpy as np

sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

from BaseTaskAI import BaseTaskAI

class LineFollowTask(BaseTaskAI):
    """ This task aligns with a line below the sub, in order to 
    """
    def __init__(self, *largs):
        """
        Takes no parameters, or a depth to hold.
        """
        BaseTaskAI.__init__(self) #initialize BaseClassAI so it can have it's communicator
        self.PUBLISHING_INTERVAL = 2.0 #seconds
            
    def run(self, *largs):  # largs unused
        while(self.active):
            #prepare packet
            packet = self.getBlankPacket()

            # if seen a line: (or at least recently)
            #       while not over a line:
            #           try to move toward it
            #           if see two lines, select which one you want
            #               disregard line that isn't selected.  (keep track, they might switch order)
            #       if still over a line but not matching:
            #           keep center, and turn to match direction
            #       if matching over a line:
            #           is stable:
            #               continue to Exit Phase
            #           not stable:
            #               settle down! just wait
            #           
            # Exit Phase:
            #       carefully move forward, checking exit angle:
            #           correct for slight mistakes.
            #           if error is too large, restart






            
            self.publishCommand(packet)
            
            time.sleep(self.PUBLISHING_INTERVAL)
        self.result = 1 # 1 for successful. 0 for failure.


""" NOTEWORTHY:
{
    "VisionPath" : 
    [
        {
            "angle1": 0.0, # Radians, 0.0 being straight ahead, +(pi/2.0) being right, -(pi/2.0) being left.
            "angle2": 0.0, # Radians, 0.0 being straight ahead, +(pi/2.0) being right, -(pi/2.0) being left.
            "center": 
            {
                "top/bottom": 0.0, # -1.0 and 1.0, 0 being center of vision, 1.0 being top edge of vision, -1.0 being bottom edge of vision.
                "left/right": 0.0 # Similar to "forward"
            },
        },
    ]
}
"""