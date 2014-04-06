# Task AI 

import threading
import time
import os
import sys

#include grapevine
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator

#Include all of our Task AI classes here
sys.path.append(os.path.abspath("tasks"))
from standby_task import StandbyTask
from foo_square_task import FooSquareTask
from dive_task import DiveTask
from line_follow_task import LineFollowTask

from task_code import TaskCode
        
        
class AIStateMachine():
    """This is the home of the AI state machine controlled by the master 
	AI.  This class instanciates sub-class AI "tasks" that run the sub
    """
    def __init__(self):
        self.current_task = None 
        self.task_com = Communicator(module_name='decision/running_task')
        self.current_task = self._define_task(StandbyTask, self.task_com)
        self.current_task_code = TaskCode.STANDBY
        self.current_task.start()
        print "YESS"
        self.com = Communicator(module_name="decision/ai_state_machine")
        self.last_packet_time = 0.0
        self.last_message = None
        
    
    #just use the functions, don't make it contemporary!! TODOOOOOO!!!
    def _define_task(self, task_class, *largs):
        """Makes a Threading object with the task inherited as well"""
        
        #create the class
        class TaskThreadClass(threading.Thread, task_class): #task and thread
            def __init__(self, com, *largs):
                threading.Thread.__init__(self) #init BOTH
                task_class.__init__(self, com, *largs)
                # -1 is the running condition
                # this is the return variable 
                self.task_status = -1
                
            def run(self):
                self.task_status = task_class.run(self)
        if  self.current_task and self.current_task.isAlive():
            return self.current_task
        return TaskThreadClass(self.task_com, *largs)
    
    
    def terminate_task(self):
        #signal termination sequence
        self.current_task.active = False
        #wait until task has finished
        print "Terminating Task"
        while self.current_task.isAlive():
            time.sleep(0.2)
        #now complete, join and return
        self.current_task.join()
        return self.current_task.result
        
        
    def begin_task(self, task_code, param_val, previous_task_result ):
        # simple 'switch' for the task code
        # quick check for alive task
        if self.current_task.isAlive():
            print "Cannot start", self.last_message.name, "when task:", self.current_task_code, "is running!"
            return
        elif task_code == TaskCode.FOO_SQUARE:
            self.current_task = self._define_task(FooSquareTask,  param_val, previous_task_result)
        elif task_code == TaskCode.STANDBY_AT_DEPTH:
            self.current_task = self._define_task(StandbyTask, param_val, previous_task_result)
        elif task_code == TaskCode.DIVE:
            self.current_task = self._define_task(DiveTask, param_val, previous_task_result)
        elif task_code == TaskCode.LINE_FOLLOW:
            self.current_task = self._define_task(LineFollowTask, param_val, previous_task_result)
        else:  # default to STANDBY for safety!!
            self.current_task = self._define_task(StandbyTask, param_val, previous_task_result)
        
        # save the code!
        self.current_task_code = task_code
        
        # start the task
        self.current_task.start()
        return
        
        
    def run(self):
        while True: #infinite loop!!!!!!!
            incoming_packet = self.com.get_last_message("decision/master_ai")
            if incoming_packet:
                message = incoming_packet["desired_state"]
            if incoming_packet and incoming_packet['timestamp'] > self.last_packet_time:
                self.last_packet_time = incoming_packet['timestamp']
                self.last_message = message
                print "GOT PACKET", message
                
                # restart Task
                if self.current_task_code == message["task_code"] and message["restart"]:
                    termination_result = self.terminate_task()
                    self.begin_task( message["task_code"], message["parameter"], termination_result )
                elif self.current_task_code != message["task_code"]:
                    termination_result = self.terminate_task()
                    self.begin_task( message["task_code"], message["parameter"], termination_result )
                    
            elif not self.current_task.isAlive():
                completion_result = self.current_task.result
                print TaskCode.get_task_name(self.current_task_code).upper(), "Task Finished with:", completion_result
                self.select_next_task(completion_result)
                
            print "."
            time.sleep(1)
            
    def select_next_task(self, completion_result):
        self.begin_task( TaskCode.STANDBY, 0, completion_result )
        
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
 
