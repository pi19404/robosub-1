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
sys.path[0] = "/home/dustin/robosub/src/util/communication/"
#sys.path.append(os.path.abspath(".."))
#sys.path.append(os.path.abspath("/communication/"))
import grapevine
sys.path[0] = "/home/dustin/robosub/src/util/logging/test/"

UPDATE_FREQUENCY = 3


def main ():

	"""
	Logger = grapevine.Communicator (
		module_name = 'util/logging/logger',
		mode = "debug",
		comm_json_path = "./mock_logger_settings.json")
		
	print Logger.get_message('util/communication/test/mock_log')
	"""

	#sys.path[0] = "/home/dustin/robosub/src/util/communication"
	#sys.path[0] = "/home/dustin/robosub/src/"
	com = grapevine.Communicator(
			module_name='util/logging/test/logger',
			comm_json_path="./mock_logger_settings.json")

	while True:
		print com.get_last_message('util/logging/test/mock_log')
		"""
		i = 0
		for msg in com.get_messages('util/communication/test/mock_log'):
			i += 1
			print msg
		print i, 'messages received'
		#com.publish_message("can you hear me now?")
		"""
		time.sleep(UPDATE_FREQUENCY)
		
		
		
	
		
		
"""
	logger = open (logging.txt, 'w')

	for module in com.listening ():
		logger.write (com.get_last_message (module))
"""
		
		


if __name__ == '__main__':
    main()
