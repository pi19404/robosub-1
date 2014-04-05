# Master AI

import time
import os
import sys

#include the task codes!
sys.path.append(os.path.abspath("tasks"))
from task_code import TaskCode

#include grapevine
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator


"""
        "desired_state": {
            "state": 0, # integer code for each state
            "name": "standby",
            "note": "Default Note"
            "restart": False
        },
"""

class MasterAI():
	"""
	Handles high level state-machine as to command more specific tasks
	"""
	def __init__(self):#, human_controller=False, controller_type=0):
		self.com = self.com = Communicator(module_name="decision/master_ai")
        
    def setupTaskStatus(self):
        self.
        
	def run(self):
		
		
	
