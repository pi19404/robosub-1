# Base Task AI
# - this should be what every task AI uses as a start


"""
return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
                "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
                "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0},
                "face_of_power": self.face_of_power}
                
"""
import argparse
import os
import sys
import threading
import time
import numpy as np
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class BaseTaskAI(threading.Thread):
	DEBUG = False
	com = None

	def __init__(self):
		#self.com = Communicator(module_name='decision/running_task')
		return
	def start(self):
		return 0
	def stop(self):
		return 0
	def publishCommand(self, packet):
		self.com.publish_message(packet)

	def isStable(self, numSamples):
		# Use standard deviation on Gyroscope and/or Accelerometer
		# to measure stability in sensor data

		# Grab N number of gyroscope values and run stddev on them
		dataX = dataY = dataZ = [];

		for _ in numSamples:
			gyro = self.com.get_last_message('datafeed/sanitized/gyroscope')
			dataX.append(gyro['gx'])
			dataY.append(gyro['gy'])
			dataZ.append(gyro['gz'])

		stdX = self.stdDev(dataX)
		stdY = self.stdDev(dataY)
		stdZ = self.stdDev(dataZ)

		

	def getDepth(self):

		lastDepth = self.com.get_last_message('datafeed/sanitized/depth')
		
		return lastDepth

	def getOrientation(self):
		# returns Orientation vector (pitch, roll, heading) in radians
		ori = {
			'pitch' : self.getPitch(),
			'roll' : self.getRoll(),
			'heading' : self.getHeading()
		}
		return ori
	def getPitch(self):
		# return pitch in radians, upwards from 'flat'
		pitch = self.com.get_last_message('decision/filtering')['pitch']
		return pitch
	def getRoll(self):
		# return roll in radians, clockwise from 'flat'
		roll = self.com.get_last_message('decision/filtering')['roll']
		return roll
	def getHeading(self):
		# return heading in radians, clockwise from Magnetic North
		heading = self.com.get_last_message('decision/filtering')['heading']
		return heading
	def getVision(self):
		vision = self.com.get_last_message('sensor/vision/fates')
		return vision
	def stdDev(self, data_samples):
		total = 0
		for el in data_samples:
			total += el

		mean = total / len(data_samples)

		covariance = 0
		for el in data_samples:
			covariance += (el - mean)*(el - mean)

		stddev = np.sqrt(covariance / (len(data_samples) - 1))
		print mean, total, len(data_samples),covariance
		return stddev


if __name__ == "__main__":
	bt = BaseTaskAI()
	print bt.stdDev([600,470,170,430,300])
