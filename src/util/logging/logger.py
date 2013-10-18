# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""

Logger

"""
"""
Plan:
	-Create communicator object for each sensor? Or just subscribe? (example: com = Communicator (module_name, mode) (use comm_json_path as testing utility)
		-Looks like create communicator object then subscribe
	-Use com.get_messages () or maybe [module].get_last_message
	-Checkout logger module for python (CMehl's suggestion)
	-Nothing will subscribe to this right?
	
	Sensors:
		Motors?
		Accelerometer
		Hydrophones?
		Gyroscope
		Batteries
		Depth Sensor
		Internal Pressure Sensor
		Internal Temperature Sensor

"""


import time
import os
import sys
#sys.path.append(os.path.abspath(".."))
#sys.path.append(os.path.abspath("/communication/"))
from util.communication.grapevine import grapevine
from grapevine import Communicator

#UPDATE_FREQUENCY = 3


def main ():
	com = grapevine.Communicator(
			module_name='util/logging/test/logger',
			comm_json_path="./mock_logger_settings.json")

	logger = open (logging.txt, 'w')

	for module in com.listening ():
		logger.write (com.get_last_message (module))
		
		


if __name__ == '__main__':
    main()
