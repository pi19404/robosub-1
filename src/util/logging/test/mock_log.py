import time
from random import random
import os
import sys
#sys.path[0] = "/home/dustin/robosub/src/util/communication/" #fix this
sys.path.append(os.path.abspath("../../../util/communication"))
#from util.communication.grapevine import grapevine #preferred convention--not quite ready yet
from grapevine import Communicator
#sys.path[0] = "/home/dustin/robosub/src/util/logging/test" #and this

UPDATE_FREQUENCY = 3


def main():
    com = grapevine.Communicator(
            module_name='util/logging/test/mock_log',
            comm_json_path="./mock_logger_settings.json")

    message = {
        'depth': {
            'x': None}}

    i = 0
    while True:
        i += 1
        message['depth']['x'] = random() % 20 * 20
        com.publish_message(message)
        print 'sent', i
        time.sleep(UPDATE_FREQUENCY)

if __name__ == '__main__':
    main()
