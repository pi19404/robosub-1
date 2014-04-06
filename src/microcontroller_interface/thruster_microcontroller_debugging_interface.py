#!/usr/bin/python

#import statements
import serial
import os
import time

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
control_byte = '\n'

#HBRIDGE_0_addr = 
#HBRIDGE_1_addr = 
#HBRIDGE_2_addr = 

class Thruster:
	
	#class members
	address = 0
	forward_code = 0
	backward_code = 0
	
	def __setattr__(self, name, value) :
		if (name == "address") :
			self.__dict__[name] = value
			self.__dict__["raw_address"] = chr(value)
		elif (name == "forward_code") :
			self.__dict__[name] = value
			self.__dict__["raw_forward_code"] = chr(value)
		elif (name == "backward_code") :
			self.__dict__[name] = value
			self.__dict__["raw_backward_code"] = chr(value)
			
			
	
	#class methods
	def send(value):
		
		#ensure input is within bounds
		if (value > 127) :
			value = 127
		elif (value < -127) :
			value = -127
			
		#set the direction
		if (value > 0) :
			function_code = raw_forward_code
		else
			function_code = raw_backward_code
		
		value = chr(abs(value))
		
		checksum = ((raw_address + function_code + value) & 127)
		
		cmd = raw_address + function_code + value + checksum
		
		#s.write(cmd)
		
	#end send()

#Main code####################################################################################


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = "/dev/ttyUSB0" #this may change, depending on what port the OS gives the microcontroller
s.baudrate = 56818      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)

thruster_X_A = Thruster()


#clear the screen
os.system('clear')

magnitude = 0
direction = 1


#Main reading loop
while 1 :

	time.sleep(.1)
	
	cmd_thruster(THRUSTER_BOW_SB, magnitude*direction)
	cmd_thruster(THRUSTER_BOW_PORT, magnitude*direction)
	cmd_thruster(THRUSTER_DEPTH_SB, magnitude*direction)
	cmd_thruster(THRUSTER_DEPTH_PORT, magnitude*direction)
	cmd_thruster(THRUSTER_STERN_SB, magnitude*direction)
	cmd_thruster(THRUSTER_STERN_PORT, magnitude*direction)
	
	magnitude += 1
	if (magnitude > 127)
		magnitude = 0
		direction = -1*direction
	
	
	
#end of reading while loop 
	
	

#close the serial port
s.close()

