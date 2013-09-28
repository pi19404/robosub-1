# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import sys
import Adafruit_BBIO.GPIO as GPIO

sys.path.append("../../..")  # Place src on path
import util.communication.grapevine as grapevine

class Blinkenlights(object):
    def __init__(self, num_pins=8):
        # We are using pins "P8_10" through "P8_17".
        self.pins = ["P8_" + str(idx) for idx in range(10, 18)]
        for pin in self.pins:
            GPIO.setup(pin, GPIO.OUT)

    def set_int(self, val):
        mask = 1
        for pin in self.pins:
            if mask & val:
                GPIO.output(pin, GPIO.HIGH)
            else:
                GPIO.output(pin, GPIO.LOW)
            mask <<= 1

def main():
    com = grapevine.Communicator(
            module_name="test/beaglebone_demo/peon",
            comm_json_path="../demo_config.json")
    blinken = Blinkenlights()

    while True:
        val = com.get_last_message("test/beaglebone_demo/patron")
        if val and val['lights']:
            blinken.set_int(int(val['lights']))

if __name__ == '__main__':
    main()

