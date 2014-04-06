# Standby Task

import time

from BaseTaskAI import BaseTaskAI

class StandbyTask(BaseTaskAI):
    """This class is a placeholder AI that does nothing, but is still
	running.  It will give 'zeros' to the pipeline below it, so that they
	will still function properly.  This will be running while divers 
	are manipulating the sub into position, or we want the sub to wait 
	for whatever reason.  If given a target depth, it will apply some 
    control for that, that stabilization will take into account. 
    Otherwise all thrusters will be off; a true standby.
	"""
    def __init__(self, *largs):
        """
        Takes no parameters, or a depth to hold.
        """
        BaseTaskAI.__init__(self) #initiallize BaseClassAI so it can have it's communicator

        self.active = True # parent class can change this status to 
                           #  gracefully exit the looping 'run' function
        self.PUBLISHING_INTERVAL = 2.0 #seconds
        
        if largs: # this means there is a depth to hold!
            self.hold_depth = True
            print "HOLDING DEPTH"
        else:
            self.hold_depth = False
        self.result = None
            
    def run(self, *largs):  #largs unused
        while(self.active):
            #prepare packet
            packet = self.getBlankPacket()
            
            if self.hold_depth: #let stabilization hold our depth
                packet["Task_AI_Movement"]["up/down"] = 0.0 
            else: #all thrusters should be OFF! (STANDBY MODE)
                packet["Task_AI_Movement"]["override"] = ["up/down", "yaw", "roll", "pitch"]
            
            self.publishCommand(packet)
            
            time.sleep(self.PUBLISHING_INTERVAL)
        self.result = 1 # 1 for successful. 0 for failure.
		

#self.target_depth - self.getFooDepth()

#self.stable_data = []
        
        #run the 'is stable' once ever second or so.
