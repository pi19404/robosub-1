# foo task that moves in a square, maybe

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


class FooSquareTask(FooToolsClass):
	def __init__(self):
		print self.isStable()

