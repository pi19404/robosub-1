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
        print "State Machine Initiated"
        self.current_task = self._define_task(StandbyTask)
        self.current_task.start()
        self.com = "HERE IS OUR COMMS WITH MASTER AI"
        
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
"""
 
