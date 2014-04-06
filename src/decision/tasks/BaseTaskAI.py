# Base Task AI
# - this should be what every task AI uses as a start


"""
return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0},
                "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0},
                "desired_velocity": {"x": 0.0, "y": 0.0, "z": 0.0},
                "face_of_power": self.face_of_power}
                
"""

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

	def __init__(self, com):
		self.com = com
		self.active = True # parent class can change this status to 
                           #  gracefully exit the looping 'run' function
		self.result = None # result variable returned by task

		self.data = []
        
	def publishCommand(self, packet):
		self.com.publish_message(packet)
		
	def isStable(self):# numSamples, limit):
		# Use standard deviation on Gyroscope and/or Accelerometer
		# to measure stability in sensor data
		all_data = self.com.get_messages('datafeed/sanitized/depth')
        
        #sample in new data
		for pt in all_data:
			self.data.append( pt.get('ax') )
			if len(self.data) > 10:
				self.data.pop() #pop off the oldest data
		std = np.std(self.data) # get dat STD slut!
        
        # decide if it is stable!
		if std > 2.0:
			return False
		else:
			return True
        """  Matt's try-hard std function
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
        """        
            

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
		pitch = self.com.get_last_message('movement/orientation')["filtered_orientation"]['pitch']  #DUSTIN CHANGE TO THIS THING!!!
		return pitch
	def getRoll(self):
		# return roll in radians, clockwise from 'flat'
		roll = self.com.get_last_message('movement/orientation')["filtered_orientation"]['roll']
		return roll
	def getHeading(self):
		# return heading in radians, clockwise from Magnetic North
		heading = self.com.get_last_message('movement/orientation')["filtered_orientation"]['heading']
		return heading
        
	def getVision(self):
		vision = self.com.get_messages('sensor/vision/cam_down')
		__unused = self.com.get_last_message('sensor/vision/cam_down')
		return vision
        
	def stdDev(self, data_samples):
		return np.std(data_samples)

	def getBlankPacket(self):
		return  {"Task_AI_Movement":
                    {
                        "override":[], # override module
                        "forward/backward": 0.0,
                        "right/left": 0.0,
                        "up/down": 0.0,
                        "yaw": 0.0,
                        "roll": 0.0,
                        "pitch": 0.0
                    }
                }

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
