# Dive Task .py

class DiveTask():   #FooToolsClass):
    """This task is for simply reaching a certain depth.  It will then finish elegantly.
	"""
    def __init__(self, goal_depth, *largs):
        """
        Takes no parameters, or a depth to hold.
        """
        self.active = True # parent class can change this status to 
                           #  gracefully exit the looping 'run' function
        self.PUBLISHING_INTERVAL = 0.5 #seconds

        self.result = None # placeholder result

        self.goal_depth = goal_depth 
            
    def run(self, *largs):  #largs unused
        while(self.active):
            #prepare packet
            packet = self.getBlankPacket()

            current_depth = self.getDepth() # from base class

            packet["Task_AI_Movement"]["up"]
            
            self.publishCommand(packet)
            
            time.sleep(self.PUBLISHING_INTERVAL)

        # If program reaches here, then it did not succeed normally
        self.result = 0 # 0 for failure
		

#self.target_depth - self.getFooDepth()