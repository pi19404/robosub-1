# line follow task

import os
import sys
import time
import numpy as np

sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

from BaseTaskAI import BaseTaskAI

class LineFollowTask(BaseTaskAI):
    """ This task aligns with a line below the sub, in order to find a task
    
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
    """
    def __init__(self, com, *largs):
        """
        Takes no parameters, or a depth to hold.
        """
        BaseTaskAI.__init__(self,com) #initialize BaseClassAI so it can have it's communicator
        self.PUBLISHING_INTERVAL = 0.2 #seconds
        
        #get anticipated angle
        if largs and largs[0]:
            self.anticipated_angle = largs[0]
        else:
            self.anticipated_angle = 45.
        self.two_angles = False
        self.last_seen = time.time()
            
    def run(self, *largs):  # largs unused
        while(self.active):
            self.best_packet = None # for determining a packet
            
            self.find_best_packet()
                        
            if self.best_packet: # see a line in general
                self.last_seen = self.best_packet["timestamp"]
                if not self.is_over_line(self.best_packet):
                    self.approach_center(self.best_packet)
                elif abs(self.select_angle()) > 5.: # degrees 
                    self.approach_center(self.best_packet)
                elif self.isStable():
                    return self.exit_sequence()
                else: # not stable! settle down
                    self.publishCommand(self.getBlankPacket()) #send 0's
            else:
                if self.last_seen + 0.5 < time.time(): #lost sight, stop moving!
                    self.publishCommand(self.getBlankPacket())
                    print "Lost sight!"
                if self.last_seen + 5.0 < time.time(): # totally lost!
                    return 0 # FAIL
                    
            time.sleep(self.PUBLISHING_INTERVAL)
        self.result = 0 # 1 for successful. 0 for failure.
    
    def exit_sequence(self):
        while(self.active):
            self.find_best_packet()
            if not self.best_packet:
                return 1 # made it out of view!
            
            self.move_from_point(self, self.best_packet, self.select_angle(self.best_packet))
            time.sleep(self.PUBLISHING_INTERVAL)
            
        self.result = 0 # 1 for successful. 0 for failure.
    
    def find_best_packet(self):
        self.best_packet = None
        vision_packets = self.getVision()
        for packet in vision_packets:
            if "VisionPath" in packet:
                if not self.best_packet:
                    self.best_packet = packet["VisionPath"]
                elif packet["timestamp"] > self.best_packet["timestamp"]:
                    self.best_packet = packet["VisionPath"]
    
    def select_angle(self, packet):
        if not "angle2" in packet:
            return packet["angle1"]
            
        a1 = packet["angle1"] - self.anticipated_angle
        a2 = packet["angle2"] - self.anticipated_angle
        if a1 < a2:
            return packet["angle1"]
        else:
            return packet["angle2"]
    
    def approach_center(self, packet):
        if "angle2" in packet:
            self.anticipated_angle = self.select_angle()
            self.two_angles = True
            self.move_to_point(self.best_packet, self.anticipated_angle)
        else: #single angle to worry about
            self.move_to_point(self.best_packet, packet["angle1"])
    
    def move_to_point(self, packet, angle):
        #prepare packet
        msg = self.getBlankPacket()
        #set positions 
        msg["Task_AI_Movement"]["forward/backward"] = packet["center"]["top/bottom"] * 1.5 
        msg["Task_AI_Movement"]["right/left"] = packet["center"]["right/left"] * 1.5
        msg["Task_AI_Movement"]["yaw"] = angle * 180./math.pi
        #publish
        self.publishCommand(msg)
    
    def move_from_point(self, packet, angle):
        #prepare packet
        msg = self.getBlankPacket()
        #set positions 
        msg["Task_AI_Movement"]["forward/backward"] = 5. #meters (medium speed?)
        msg["Task_AI_Movement"]["yaw"] = (angle * 180./math.pi )/ 5. #subtle movements!
        self.publishCommand(msg)
    
    def is_over_line(self, packet):
        if abs(packet["center"]["top/bottom"]) > 0.2:
            return False
        if abs(packet["center"]["left/right"]) > 0.2:
            return False
        return True
        
        
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
