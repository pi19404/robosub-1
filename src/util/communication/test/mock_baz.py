# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
sys.path.append(os.path.abspath(".."))
import grapevine

UPDATE_FREQUENCY = 1

def main():
    com = grapevine.Communicator(
            module_name='util/communication/test/mock_baz',
            comm_json_path="./mock_comm_settings.json")

    while True:
        #print com.get_last_message('util/communication/test/mock_foo')
        for msg in com.get_messages('util/communication/test/mock_foo'):
            print msg
        for msg in com.get_messages('util/communication/test/mock_bar'):
            print msg
        time.sleep(UPDATE_FREQUENCY)

if __name__ == '__main__':
    main()

