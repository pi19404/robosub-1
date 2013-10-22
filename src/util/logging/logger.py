#COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""

Logger

"""

from time import sleep
import os
import sys
sys.path.append(os.path.abspath("../../util/communication"))
#from util.communication.grapevine import Communicator #preffered method
from grapevine import Communicator

UPDATE_FREQUENCY= 1

def main ():

	com = Communicator(
		module_name='util/logging',
			comm_json_path="./communication_settings.json")

##################################################
# This code outputs a file for each sensor       #
##################################################

	i = 0
	while True:
		for sensor in com.listening(): #is constantly querying this an efficient way of doing this?
			#filename = str (str (sensor).replace ('/', '-') + '.log') #replace() is needed because '/'s are not allowed in file names
			filename = str (str (sensor [sensor.rfind ('/') + 1:]) + '.log') #this strips out everything before and including the last '/'
			with open (filename, 'a') as sensor_out: #is constantly opening and reopening the file an efficient way of doing this?
				sensor_out.write (str (com.get_last_message (sensor)) + '\n')
			print 'Recieved', i, 'messages for each module'
			i += 1
		sleep (UPDATE_FREQUENCY)
		
###################################################
# This code outputs a single file for all sensors #
###################################################
"""
	logger = open ("logging.log", 'w')

	while True:
		for module in com.listening():
			message = com.get_last_message (module)
			print message
			logger.write (str (message) + '\n')
		logger.write ('\n')
		sleep (UPDATE_FREQUENCY)

	logger.close()
"""

if __name__ == "__main__":
	main()
