# Task AI 

import threading
import time
import os
import sys

#Include all of our Task AI classes here
sys.path.append(os.path.abspath("tasks"))
from StandbyTask import StandbyTask
from FooSquareTask import FooSquareTask

#include grapevine
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator
        
        
class AIStateMachine():
    """This is the home of the AI state machine controlled by the master 
	AI.  This class instanciates sub-class AI "tasks" that run the sub
    """
    def __init__(self):
        self.current_task = self._define_task(StandbyTask)
        self.current_task.start()
        print "YESS"
        self.com = Communicator(module_name="decision/ai_state_machine")
        
    #just use the functions, don't make it contemporary!! TODOOOOOO!!!
    def _define_task(self, task_class, *largs):
        """Makes a Threading object with the task inherited as well"""
        
        #create the class
        class TaskThreadClass(threading.Thread, task_class): #task and thread
            def __init__(self, *largs):
                threading.Thread.__init__(self) #init BOTH
                task_class.__init__(self, *largs)
                # -1 is the running condition
                # this is the return variable 
                self.task_status = -1
                
            def run(self):
                self.task_status = task_class.run(self)
                
        return TaskThreadClass()
        
        
    def run(self):
        #TODO
        self.current_task.active = False
        print "hopefully all done"
        
        

if __name__ == "__main__":
    a = AIStateMachine()
    a.run()
    print "DONE"






"""
Robosub Notes LOL

For Sunday Pool Test
* Start Gate
* Line Following

TONIGHT-> hammer out packets

>> other peeps
	> camera testing
    
PUBLISHING:YESS
 {'Task_AI_Movement': {'override': ['up', 'pitch_up', 'roll_right', 'heading'], 'position': {'forward': 0.0, 'right': 0.0, 'up': 0.0}, 'orientation': {'roll_right': 0.0, 'pitch_up': 0.0, 'heading': 0.0}}}
Traceback (most recent call last):
  File "ai_state_machine.py", line 55, in <module>
    a = AIStateMachine()
  File "ai_state_machine.py", line 26, in __init__
    self.com = Communicator(module_name="ai_state_machine")
  File "/home/chris/Documents/robosub/src/util/communication/grapevine.py", line 78, in __init__
    self.publisher['socket'].bind(get_socket_name(self.module_name))
  File "/home/chris/Documents/robosub/src/util/communication/grapevine.py", line 27, in get_socket_name
    return 'tcp://{ip}:{port}'.format
    
"""
 
