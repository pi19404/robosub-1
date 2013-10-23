#COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""
Logger Module
	This listens to the various sensors and modules and outputs a log for each
"""

"""
	Sensors logger listens to (can be found in src/communication_settings.json):
		Decision Module
		Directive Module
		Stabilization Module
		Physical Module
		Vision Module
		Hydrophone
		Depth Sensor
		Accelerometer
		Speed Sensor
"""


from time import sleep
from time import time
import os
import sys
sys.path.append(os.path.abspath("../../../util/communication"))
#from util.communication.grapevine import Communicator #preffered method
from grapevine import Communicator

UPDATE_FREQUENCY= 3


def main ():

	com = Communicator(
		module_name='util/logging/test/logger',
			comm_json_path="./mock_logger_settings.json")

#####################################################################
# This is a more efficient way of outputting a file for each sensor #
#####################################################################

	log_files = {} 
	for sensor in com.listening(): #populates a dict with the module and its associated .log file
		log_files[sensor] = open (str (str (sensor [sensor.rfind ('/') + 1:]) + '.log'), 'w') #this strips out everything before and including the last '/'

	i = 0
	while True:
		for sensor_out in log_files:
			message = com.get_last_message (sensor_out)
			if not message: #will output sensor off if logger updates faster than the sensor
				log_files[sensor_out].write ("Sensor off at time: " + str (time() ) )
			else:
				for k in message.keys():
					log_files[sensor_out].write (str (k) + ':' + str (message[k]) + ' ',) #makes the .log file a bit more readable
			log_files[sensor_out].write ('\n')

		print 'Checked for messages ' + str (i) + ' times'
		i += 1
		sleep (UPDATE_FREQUENCY)

	for sensor_file in log_files:
		log_files[sensor_file].close()

##################################################
# This code outputs a file for each sensor       #
##################################################

"""
	i = 0
	while True:
		for sensor in com.listening(): #is constantly querying this an efficient way of doing this?
			#filename = str (str (sensor).replace ('/', '-') + '.log') #replace() is needed because '/'s are not allowed in file names
			filename = str (str (sensor [sensor.rfind ('/') + 1:]) + '.log') #this strips out everything before and including the last '/'

			with open (filename, 'a') as sensor_out: #is constantly opening and reopening the file an efficient way of doing this?
				#sensor_out.write (str (com.get_last_message (sensor)) + '\n')
				message = com.get_last_message (sensor)

				if not message: #will output sensor off if logger updates faster than the sensor
					sensor_out.write ("Sensor off at time: " + str (time() ) )
				else:
					for k in message.keys():
						sensor_out.write (str (k) + ':' + str (message[k]) + ' ',) #makes the .log file a bit more readable

				sensor_out.write ('\n')

		print 'Checked for messages' + str (i) + 'times'
		i += 1
		sleep (UPDATE_FREQUENCY)
"""	
	
###################################################
# This code outputs a single file for all sensors #
###################################################
"""
	logger = open ("logging.log", 'w')

	while True:
		for module in com.listening():
			message = com.get_last_message (module)
			#print message
			logger.write (str (message) + '\n')
		logger.write ('\n')
		sleep (UPDATE_FREQUENCY)

	logger.close()
"""

###################################################


def shorten_filename (sensor):
	return str (str (sensor [sensor.rfind ('/') + 1:]) + '.log') #this strips out everything before and including the last '/' and stores it in a list





if __name__ == "__main__":
	main()
