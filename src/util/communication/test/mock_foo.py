# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
from random import random
sys.path.append(os.path.abspath(".."))
import grapevine

UPDATE_FREQUENCY = 0.1

def main():
    com = grapevine.Communicator(
            module_name='util/communication/test/mock_foo',
            comm_json_path="./mock_comm_settings.json")

    message = {
        'move': {
            'x': None}}

    i = 0
    while True:
        i += 1
        message['move']['x'] = random() * 2 - 1
        com.publish_message(message)
        print 'sent', i
        time.sleep(UPDATE_FREQUENCY)
        recv_msg = com.get_last_message('util/communication/test/mock_bar')
        if recv_msg:
            print recv_msg

if __name__ == '__main__':
    main()

