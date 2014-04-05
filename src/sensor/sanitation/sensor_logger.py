# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""Sensor Data Logging module

This module subscribes to the sanitized data from sanitation.py and prints it to the screen

"""

import argparse
import os
import sys
import threading
import time
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

def main:
	#Establish a communicator object
	com = Communicator(module_name="sensor/sanitation/log")

	while True:
		accel = com.get_last_message("datafeed/sanitized/accelerometer")
		gyro = com.get_last_message("datafeed/sanitized/gyroscope")
		depth = com.get_last_message("datafeed/sanitized/depth")
		voltage = com.get_last_message("datafeed/sanitized/battery_voltage")
		compass = com.get_last_message("datafeed/sanitized/compass")

		os.system("clear")
		
		print "Accel X: {0}".format(accel.get('ax'))
		print "Accel Y: {0}".format(accel.get('ay'))
		print "Accel Z: {0}".format(accel.get('az'))

		print "Gyro X: {0}".format(gyro.get('gx'))
		print "Gyro Y: {0}".format(gyro.get('gy'))
		print "Gyro Z: {0}".format(gyro.get('gz'))

		print "Depth: {0}".format(depth.get('value'))

		print "Voltage: {0}".format(voltage.get('voltage'))

		print "Compass X: {0}".format(compass.get('cx'))
		print "Compass Y: {0}".format(compass.get('cy'))
		print "Compass Z: {0}".format(compass.get('cz'))




if __name__ == '__main__':
    main()
