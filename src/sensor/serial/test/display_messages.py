# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
from random import random
sys.path.append(os.path.abspath("../../.."))
from util.communication.grapevine import Communicator

EPOCH = 0.01

def main():
    com = Communicator(module_name='sensor/serial/test/display_messages')

    while True:
        for module_name in com.listening():
            recv_msg = com.get_last_message(module_name)
            if recv_msg:
                print recv_msg
        time.sleep(EPOCH)

if __name__ == '__main__':
    main()

