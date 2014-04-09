#!/usr/bin/python
"""
This is the microcontroller library. All classes and functions that interact
with the microcontrollers that might be shared across different micrcocontroller
modules should (eventually) go in here.
"""

class Thruster:
	"""
	This class is used for controlling the thrusters. An instance should be
	made for each thruster.

	Initialization Example:
		mthruster = Thruster(37, 1)
		
	Several parameters must be passed when initializing the thruster.
	
	Address:
		The address of the H-bridge that the thruster is connected to.
	H-bridge Port:
		The port of the  H-bridge that the thruster is connected to.
		Valid values are 1 and 2.
	Serial Port:
		A handle to a serial port with which it can communicate to the thrusters
	

	Usage:s
		Once initialized, thruster commands can be sent using the .send() method.
	
	
	
	***How the Thrusters/H-bridges work***
			(if you're curious)
	
	The thrusters are controlled by the H-bridges, which has a specific
	control protocol. Each H-bridge controls 2 thrusters (named 1 and 2,
	respectively). To control a thruster, one must send the address of the 
	H-bridge, a function code(explained later), the magnitude of the thruster 
	value, and a special checksum computed of the previous parameters.
	
	H-bridge communication parameters:
	
	Address:
		The address corresponds to the H-bridge controlling the thruster,
		not the thruster itself. For six thrusters, there are 3 H-bridges. 
		Therefore, each thruster will share the same address as one other
		thruster.
	
	Function Code:
		There are 4 function codes that we use. Look at the datasheet for
		more information about other function codes.
		
		Code 0:
			Drive motor 1 forwards
		Code 1:
			Drive motor 1 backwards
		Code 4:
			Drive motor 2 forwards
		Code 5:
			Drive motor 2 backwards
	
	Magnitude:
		The value to be sent to the H-bridge. Valid values are between 0 and 127.
		These will cause a thruster to go forwards or backwards depending on the
		function code that is sent. With either function code, a magnitude of 0 will
		cause the thruster to stop.
	
	Checksum:
		The checksum is used by the H-bridge to validate sent data. The checksum is
		calculated as follows:
			checksum = (Addresss + Function Code + Magnitude) & 127	
	"""
	
	#external class members
	address = 0
	h_bridge_port = 0
	serial_port = 0
	
	#internal class members (you shouldn't touch these)
	#raw_forward_code
	#raw_backward_code
	
	def __init__(self, address, h_bridge_port, serial_port) :
		self.address = address
		self.h_bridge_port = h_bridge_port
		self.serial_port = serial_port
	
	def __setattr__(self, name, value) :
		if (name == "address") :
			self.__dict__[name] = value
			self.__dict__["raw_address"] = chr(value)
			
		elif (name == "h_bridge_port") :
			if (value == 1) :
				self.__dict__[name] = value
				self.__dict__["raw_forward_code"] = chr(0)
				self.__dict__["raw_backward_code"] = chr(1)
			elif (value == 2) :
				self.__dict__[name] = value
				self.__dict__["raw_forward_code"] = chr(4)
				self.__dict__["raw_backward_code"] = chr(5)
			else :
				print "Error: Invalid H-bridge port selected"
				
		if (name == "serial_port") :
			self.__dict__[name] = value
	
	def __repr__ (self):
	
		mstr = "Address: " + str(self.address) + '\n' \
			   "H-bridge Port:    " + str(self.h_bridge_port) + '\n'
			   
		return mstr			
	
	#class methods
	def send(self, value):
		
		#ensure input is within bounds
		if (value > 127) :
			value = 127
		elif (value < -127) :
			value = -127
			
		#set the direction
		if (value > 0) :
			raw_function_code = self.raw_forward_code
		else :
			raw_function_code = self.raw_backward_code
		
		# calculate absolute value
		value = abs(value)
		
		#calculate the checksum
		checksum = ( (ord(self.raw_address) + ord(raw_function_code) + value) & 127)
		
		#generate the entire command
		cmd = self.raw_address + raw_function_code + chr(value) + chr(checksum)
		
		self.serial_port.write(cmd)
		
	#end send()
#end thruster class