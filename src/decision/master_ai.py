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
        self.com= Communicator(module_name="decision/master_ai")
        self.com.publish_message(self.getBlankMessage())
    
    def getBlankMessage(self):
        return {
                "desired_state": {
                    "task_code": 0, # integer code for each state
                    "name": "standby",
                    "note": "Default Note",
                    "restart": False,
                    "parameter": None
                    }
                }
    def run(self):
        while True:
            packet = self.getBlankMessage()
            packet["desired_state"]["restart"] = True
            self.com.publish_message(self.getBlankMessage())
            time.sleep(2.)
		
m = MasterAI()
m.run()




"""

 {'roll_right': 0.0, 'pitch_up': 0.0, 'heading': 0.0}}}
GOT PACKET {'module_name': 'decision/master_ai', 'desired_state': {'task_code': 0, 'note': 'Default Note', 'parameter': None, 'name': 'standby', 'restart': False}, 'message_number': 3, 'timestamp': 1396659765.895823}
Traceback (most recent call last):
  File "ai_state_machine.py", line 103, in <module>
    a.run()
  File "ai_state_machine.py", line 89, in run
    if self.current_task_code == incoming_packet.task_code and incoming_packet.restart:
AttributeError: 'dict' object has no attribute 'task_code'


"""
