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

    x_val = 0
    i = 0
    while True:
        i += 1
        accel_vals = com.get_last_message("test/beaglebone_demo/minion")

        if accel_vals:
            x = accel_vals['x_val']
            if x < 0.75:
                x_val = 0b00001
            elif 0.75 <= x and x < 0.8:
                x_val = 0b00010
            elif 0.8 <= x and x <= 0.9:
                x_val = 0b00100
            elif 0.90 <= x and x <= 0.95:
                x_val = 0b01000
            else:
                x_val = 0b10000
            print x, x_val

        com.publish_message({'x_val': x_val, 'y_val': i})
        #print accel_vals
        time.sleep(0.05)  # Any faster and we can't see the bottom light.

if __name__ == '__main__':
    main()

