####  Sanitation Module (sanitation.py)
#####	
#####	This module subscribes to all the sensors and uses the sensor data to create
#####	a organized human-readable python dictionary for use throughout the system
#####

import json
import zmq
import os
import sys
import datetime
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator

san = Communicator(module_name="sensor/sanitation")
sensors = {}
# Example Sensor Object
#
# sensors = {
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
def fetchData(obj, baseData=None):
	# First get all Gyroscope and Accelerometer messages since they come seperately
		gyro = {
			'gx':None,
			'gy':None,
			'gz':None
		}
		
		while True:
			gyro_msg = san.get_last_message("sensor/gyroscope")
			if gyro_msg:
				if gyro['gx'] is None:
					gyro['gx'] = gyro_msg.get('GYRO_X')

				if gyro['gy'] is None:
					gyro['gy'] = gyro_msg.get('GYRO_Y')

				if gyro['gz'] is None:
					gyro['gz'] = gyro_msg.get('GYRO_Z')
			else:
				print "Cannot get data"	
				return None
			if gyro['gx'] is None or gyro['gy'] is None or gyro['gz'] is None:
				continue
			else:
				# if we have data for all axises then adjust for calibration and break;
				if baseData != None:
					gyro['gx'] -= baseData['gyroscope']['gx']
					gyro['gy'] -= baseData['gyroscope']['gy']
					gyro['gz'] -= baseData['gyroscope']['gz']
				break

		obj['gyroscope'] = gyro

		# Now accelerometer
		accel = {
			'ax':None,
			'ay':None,
			'az':None
		}

		while True:
			accel_msg = san.get_last_message("sensor/accelerometer")
			if accel_msg:
				if accel['ax'] is None:
					accel['ax'] = accel_msg.get('ACL_X')

				if accel['ay'] is None:
					accel['ay'] = accel_msg.get('ACL_Y')

				if accel['az'] is None:
					accel['az'] = accel_msg.get('ACL_Z')
				
				if accel['ax'] is None or accel['ay'] is None or accel['az'] is None:
					continue
				else:
					# if we have data for all axises then break;
					if baseData != None:
						accel['ax'] -= baseData['accelerometer']['ax']
						accel['ay'] -= baseData['accelerometer']['ay']
						accel['az'] -= baseData['accelerometer']['az']
					break

		obj['accelerometer'] = accel


		# Now lets get the depth using the conversion function
		# y = 0.1075x - 54.622
	
		depth_msg = san.get_last_message("sensor/depth")
		if depth_msg:
			depth_raw = depth_msg.get('DEPTH')
			if depth_raw is None:
				print 'Error getting depth value'
			else:

				depth_actual = 0.1075 * depth_raw - 54.622
	
				if baseData != None:
					depth_actual -= baseData['depth']['value']

				depth =  {'value' : depth_actual}

				obj['depth'] = depth

		# And lastly, battery voltage
		batt_msg = san.get_last_message("sensor/battery_voltage")
		if batt_msg:
			batt_raw = batt_msg.get('BATTERY_VOLTAGE')
			if batt_raw is None:
				print 'Error getting battery value'
			else:
				
			
				battery = {'voltage' : batt_raw }

				obj['battery_voltage'] = battery
		
		

		# We will add compass later
		#comp_msg = san.get_last_message("sensor/compass")
	
def calibrateData(dataSampleSize=10):
	"""---------------------------
		This function is used to calibrate the data upon placing the sub into water.

		1) I will collect N number of data points from the Microcontroller
		2) I will then find the average of all N data points
		3) There will be a base sensors object that will act as a relative calibration metric to define base values for the sub
		
	---------------------------"""

	dataPool = [] #Used to store list of data objects
	
	percentComplete = 0
	dataCounter = 0
	for i in range(0 , dataSampleSize):
		dataObj = {}
		percentComplete = float(float(i) / dataSampleSize * 100)
		os.system("clear")
		print "Starting Calibration..."
		print percentComplete, "% complete..."		
		ret = fetchData(dataObj)
		if ret is None:
			print "Cannot connect to microcontroller"
			return None
		dataPool.append(dataObj)
		++dataCounter
		
		
	# Now we have a list of N data points for each sensor
	

	# So lets go through each and build up the average
	averageObj = {
		'accelerometer' : {
			'ax' : 0,
			'ay' : 0,
			'az' : 0
		},
		'gyroscope' : {
			'gx' : 0,
			'gy' : 0,
			'gz' : 0
		},
		'depth' : {
			'value':0
		},
		'battery_voltage' : {
			'voltage': 0
		},
		'compass' : {
			'direction':0
		}
	}

	ax = ay = az = gx = gy = gz = depth = voltage = 0

	for obj in dataPool:
		ax += obj['accelerometer']['ax']
		ay += obj['accelerometer']['ay']
		az += obj['accelerometer']['az']	

		gx += obj['gyroscope']['gx']	
		gy += obj['gyroscope']['gy']
		gz += obj['gyroscope']['gz']

		depth += obj['depth']['value']

		voltage += obj['battery_voltage']['voltage']
	
	averageObj['accelerometer']['ax'] = ax/dataSampleSize
	averageObj['accelerometer']['ay'] = ay/dataSampleSize
	averageObj['accelerometer']['az'] = az/dataSampleSize
	averageObj['gyroscope']['gx'] = gx/dataSampleSize
	averageObj['gyroscope']['gy'] = gy/dataSampleSize
	averageObj['gyroscope']['gz'] = gz/dataSampleSize
	averageObj['depth']['value'] = depth/dataSampleSize
	averageObj['battery_voltage']['voltage'] = voltage/dataSampleSize
	print
	print "Calibration complete..."
	os.system("sleep 1")
	print 
	print "----------- Base sensor object ---------"
	print averageObj
	return averageObj	

def main():
	willCalibrate = True
	
	if len(sys.argv) == 1:
		arg = "100"
	elif len(sys.argv) == 2:
		
		if sys.argv[1].isdigit():
			arg = sys.argv[1]
		elif sys.argv[1] == "--NOCAL":
			willCalibrate = False
			arg = "0"
		else:
			print "usage: ", sys.argv[0], " NUMBER [--NOCAL]"
			return
	else:
		print "usage: ", sys.argv[0], " NUMBER [--NOCAL]"
		return

	# We need a method to calibrate the data

	if arg.isdigit():
		numCalibrations = int(arg)
	
	if willCalibrate:
		baseData = calibrateData(numCalibrations)
	else:
		baseData = None
		
	dataCounter = 0
	percentComplete = 0
	while True:
		# Get data
		ret = fetchData(sensors, baseData)
		
		if ret is None:
			print "Cannot connect to microcontroller"
			return
		
		# Finally, we will publish our sensors object to the grapevine for use elsewhere
		os.system("clear")
		print "Accelerometer"
		print "AX: %d" % sensors['accelerometer']['ax']
		print "AY: %d" % sensors['accelerometer']['ay']
		print "AZ: %d" % sensors['accelerometer']['az']
		print
		print "Gyroscope"
		print "GX: %d" % sensors['gyroscope']['gx']
		print "GY: %d" % sensors['gyroscope']['gy']
		print "GZ: %d" % sensors['gyroscope']['gz']
		print
		print "Depth: %d" % sensors['depth']['value']
		print "Voltage: %d" % sensors['battery_voltage']['voltage']
		
		
		san.publish_message(sensors)



if __name__ == '__main__':
	main()
