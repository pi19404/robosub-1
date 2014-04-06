# Standby Task

import time


#TODO: import the AI tools class
class FooToolsClass():
    """
	    "Task_AI_Movement":
    {
        "override":["up"] # override module
        "position": {
            "right": 0.0
            "forward": 0.0
            "up": 0.0
        }
        "orientation": { # Notably, this is the desired state, not a requested change in state
            "pitch_up": 0.0, 
            "roll_right": 0.0,
            "heading": 0.0  #looking down, Clockwise, in radians
        }
    }
    
    """
    	
    def isStable(self):
        return True # Normally this would do something interesting
	
    def publishCommand(self, cmd):
        print "PUBLISHING:", cmd
    
    def getFooDepth(self):
        return 2.3 #meters
    
    def getBlankPacket(self):
        return  {"Task_AI_Movement":
                    {
                        "override":[] # override module
                        "forward/backward": 0.0,
                        "right/left": 0.0,
                        "up/down": 0.0,
                        "yaw": 0.0,
                        "roll": 0.0,
                        "pitch": 0.0
                    }
                }

class StandbyTask(FooToolsClass):
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
                packet["Task_AI_Movement"]["up"] = 0.0 
            else: #all thrusters should be OFF! (STANDBY MODE)
                packet["Task_AI_Movement"]["override"] = ["up","pitch_up","roll_right","heading"]
            
            self.publishCommand(packet)
            
            time.sleep(self.PUBLISHING_INTERVAL)
        self.result = 1 # 1 for successful. 0 for failure.
		

#self.target_depth - self.getFooDepth()
