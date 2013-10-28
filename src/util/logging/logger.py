#COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""
Logger Module
	This listens to the various sensors and modules and outputs a log for each
"""

"""
Sensors this module listens to can be found in robosub/src/communication_settings.json
"""


from time import sleep
from time import time
import os
import sys
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator #preffered method

UPDATE_FREQUENCY= 0.5


def main ():
	#initialize communicator using grapevine.py
	com = Communicator(
		module_name='util/logging')
			#comm_json_path="../../communication_settings.json")

	#open log files
	log_files = {} #dict containing the sensor as key and file as value. Ex: log_files['sensor/serial/accelerometer']=file ("accelerometer")
	for sensor in com.listening():
		log_files[sensor] = open (str (str (sensor [sensor.rfind ('/') + 1:]) + '.log'), 'w') #this strips out everything before and including the last '/' from the filename

	#output to log files
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

	#close log files
	for sensor_file in log_files:
		log_files[sensor_file].close()



if __name__ == "__main__":
	main()
