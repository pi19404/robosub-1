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

class BaseTaskAI():
	DEBUG = False
	com = None

	def __init__(self):
		self.com = Communicator(module_name='decision/running_task')
		return
	def publishCommand(self, packet):
		self.com.publish_message(packet)
		
	def getBlankPacket(self):
	        return {
	            "Task_AI_Movement": {
	                "override":[], # override module (to hold strings
	                "position": {
	                    "right": 0.0,
	                    "forward": 0.0,
	                    "up": 0.0
	                },
	                "orientation": { # Notably, this is the desired state, not a requested change in state
	                    "pitch_up": 0.0, 
	                    "roll_right": 0.0,
	                    "heading": 0.0  #looking down, Clockwise, in radians
	                    }
	                }
	            }
	def isStable(self, numSamples, limit):
		# Use standard deviation on Gyroscope and/or Accelerometer
		# to measure stability in sensor data

		# Grab N number of gyroscope values and run stddev on them
		dataX = dataY = dataZ = [];

		for _ in numSamples:
			self.com.get_messages()
			gyro = self.com.get_last_message('datafeed/sanitized/gyroscope')
			dataX.append(gyro['gx'])
			dataY.append(gyro['gy'])
			dataZ.append(gyro['gz'])

		stdX = self.stdDev(dataX)
		stdY = self.stdDev(dataY)
		stdZ = self.stdDev(dataZ)

		if stdX < limit and stdY < limit and stdZ < limit:
			return True
		return False
		

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
		return np.std(data_samples)


if __name__ == "__main__":
    bt = BaseTaskAI()
    i = 0
    data = []
    prevTime = 0

    while True:
        val = bt.com.get_last_message('datafeed/sanitized/accelerometer')
      
        if val is not None:
            dt = val.get('ax')
            timestamp = val.get('timestamp')
            
            if timestamp != prevTime:
                prevTime = timestamp
                data.append(dt)
                if(len(data) >= 10):
			        break
          
    print bt.stdDev(data)
