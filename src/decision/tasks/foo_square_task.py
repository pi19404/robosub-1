# foo task that moves in a square, maybe
import time
from BaseTaskAI import BaseTaskAI

class FooSquareTask(BaseTaskAI):
    def __init__(self, com, *largs):
        BaseTaskAI.__init__(self, com)
        self.PUBLISHING_INTERVAL = 0.5 #seconds
        self.sequence_count = 0
        
    def run(self, *largs):  #largs unused
        while(self.active):
            #prepare packet
            packet = self.getBlankPacket()
            
            
            if self.sequence_count > 100: #finish up early
                self.result = 1
                self.publishCommand(packet)
                return
            elif self.sequence_count % 2:
                packet["Task_AI_Movement"]["right/left"] = 45.
                print "turning"
                time.sleep(1.)
            else:
                packet["Task_AI_Movement"]["forward/backward"] = 10.
                print "going forward"
                time.sleep(3.)
            
            self.publishCommand(packet)
            self.sequence_count = self.sequence_count + 1

        # If program reaches here, then it did not succeed normally
        self.result = 0 # 0 for failure

