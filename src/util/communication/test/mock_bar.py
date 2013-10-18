# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
sys.path.append(os.path.abspath(".."))
import grapevine

UPDATE_FREQUENCY = 3

def main():
    com = grapevine.Communicator(
            module_name='util/communication/test/mock_bar',
            comm_json_path="./mock_comm_settings.json")

    while True:
        #print com.get_last_message('util/communication/test/mock_foo')
        i = 0
        for msg in com.get_messages('util/communication/test/mock_foo'):
            i += 1
            print msg
        print i, 'messages received'
        com.publish_message("can you hear me now? No really can you hear me?")
        time.sleep(UPDATE_FREQUENCY)

if __name__ == '__main__':
    main()

