# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import sys
from random import random
sys.path.append("../../..")  # Place src on path
import util.communication.grapevine as grapevine

def main():
    com = grapevine.Communicator(
            module_name='test/beaglebone_demo/patron',
            comm_json_path="../demo_config.json")

    i = 0
    while True:
        i += 1
        com.publish_message({'x_val': i, 'y_val': i})
        time.sleep(0.05)  # Any faster and we can't see the bottom light.

if __name__ == '__main__':
    main()

