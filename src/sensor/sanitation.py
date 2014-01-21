####  Sanitation Module (sanitation.py)
#####	
#####	This module subscribes to all the sensors and uses the sensor data to create
#####	a organized human-readable python dictionary for use throughout the system
#####

import json
import zmq
import os
import datetime
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator

sensors = {}
# Example Sensor Object
#
# sensors = {
# 	'timestamp' : datetime.datetime.now(),
# 	'gyroscope' : {
# 		'gx' : 1,
# 		'gy' : 2,
# 		'gz' : 3
# 	},
# 	'accelerometer' : {
# 		'ax' : 1,
# 		'ay' : 2,
# 		'az' : 3
# 	},
# 	'battery_voltage' :  {
# 		'voltage' : 5
# 	},
# 	'depth' : {
# 		'value' : 10
# 	},
# 	'compass' : {
# 		'direction' : 0
# 	}
# }

def main():
	san = Communicator(module_name="sensor/sanitation")

	while True:
		# Get all of the last messages

		# First get all Gyroscope and Accelerometer messages since they come seperately
		gyro = {}
		
		while True:
			gyro_msg = san.get_last_message("sensor/gyroscope")
			gyro.gx = gyro_msg.get('GYRO_X')
			gyro.gy = gyro_msg.get('GYRO_Y')
			gyro.gz = gyro_msg.get('GYRO_Z')
			if gyro.gx is None or gyro.gy is None or gyro.gz is None:
				continue
			else
				# if we have data for all axises then break;
				break;

		sensors['gyroscope'] = gyro

		# Now accelerometer
		accel = {}

		while True:
			accel_msg = san.get_last_message("sensor/accelerometer")
			accel.ax = accel_msg.get('ACL_X')
			accel.ay = accel_msg.get('ACL_Y')
			accel.az = accel_msg.get('ACL_Z')
			if accel.ax is None or accel.ay is None or accel.az is None:
				continue
			else
				# if we have data for all axises then break;
				break;

		sensors['accelerometer'] = accel


		# Now lets get the depth using the conversion function
		# y = 0.1075x - 54.622
	
		depth_msg = san.get_last_message("sensor/depth")
		depth_raw = depth_msg.get('DEPTH')
		if depth_raw is None:
			print 'Error getting depth value'

		depth_actual = 0.1075 * depth_raw - 54.622

		depth =  {'value' : depth_actual}

		sensors['depth'] = depth

		# And lastly, battery voltage
		batt_msg = san.get_last_message("sensor/battery_voltage")
		batt_raw = batt_msg.get('BATTERY_VOLTAGE')
		if batt_raw is None:
			print 'Error getting battery value'

		battery = {'voltage' : batt_raw }

		sensors['battery_voltage'] = battery

		# We will add compass later
		#comp_msg = san.get_last_message("sensor/compass")

		# Finally, we will publish our sensors object to the grapevine for use elsewhere
		san.publish_message(sensors)


if __name__ == '__main__':
	main()