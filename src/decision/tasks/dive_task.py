# Dive Task .py

import time
from BaseTaskAI import BaseTaskAI


class DiveTask(BaseTaskAI):
    """This task is for simply reaching a certain depth.  It will then finish elegantly.
	"""
    def __init__(self, com, goal_depth, *largs):
        """
        Takes no parameters, or a depth to hold.
        """
        BaseTaskAI.__init__(self, com) #initiallize BaseClassAI so it can have it's communicator

        self.PUBLISHING_INTERVAL = 0.5 #seconds
        self.depth_threshold = 0.5 #meters
        
        self.goal_depth = goal_depth 
            
    def run(self, *largs):  #largs unused
        while(self.active):
            #prepare packet
            packet = self.getBlankPacket()

            current_depth = self.getDepth() # from base class
            
            print "cur:", current_depth, "goal", self.goal_depth
            if not current_depth:
                current_depth = 0.5
            #for debug purposes, making it a number type
            if not current_depth:
                current_depth = 1.0
            if not self.goal_depth:
                self.goal_depth = 2.0
            
            if abs(self.goal_depth-current_depth) < (self.depth_threshold/2.0): #TODO stabilize
                if self.isStable():
                    self.result = 1
                    return 1 #Successfully reached our specified depth
                else: # not stable! settle down
                    self.publishCommand(self.getBlankPacket()) #send 0's
                    

            # still traveling  :p
            packet["Task_AI_Movement"]["up"] = self.goal_depth-current_depth
            
            self.publishCommand(packet)
            print "Diving", self.goal_depth-current_depth, "meters"
            
            time.sleep(self.PUBLISHING_INTERVAL)

        # If program reaches here, then it did not succeed normally
        self.result = 0 # 0 for failure
		

#self.target_depth - self.getFooDepth()
