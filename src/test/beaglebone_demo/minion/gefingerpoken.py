# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import sys
import Adafruit_BBIO.ADC as ADC
import time

sys.path.append("../../..")  # Place src on path
import util.communication.grapevine as grapevine

class Gefingerpoken(object):
    def __init__(self):
        ADC.setup()
        # We are using pins "P8_10" through "P8_17".
        self.x_pin = "P9_40"
        self.y_pin = "P9_39"
        self.z_pin = "P9_38"

    def prod(self, sanitize=True):
        return ADC.read(self.x_pin), ADC.read(self.y_pin), ADC.read(self.z_pin)

def main():
  # com = grapevine.Communicator(
  #         module_name="test/beaglebone_demo/minion",
  #         comm_json_path="../demo_config.json")
    finger = Gefingerpoken()

    while True:
  #     val = com.get_last_message("test/beaglebone_demo/patron")
  #     if val and val['lights']:
  #         blinken.set_int(int(val['lights']))
        print "x: {0:.3} y: {1:.3} z: {2:.3}".format(*finger.prod())
        time.sleep(0.1)

if __name__ == '__main__':
    main()

