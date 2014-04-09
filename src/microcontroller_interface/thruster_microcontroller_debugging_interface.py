#!/usr/bin/python

#import statements
import serial
import os
import sys
import time
from microcontroller import Thruster

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
control_byte = '\n'

#Main code####################################################################################

if (sys.platform == "win32") : #windows
	CLEAR_CMD = "cls"
else : 				   #some other OS (should insert more checks)
	CLEAR_CMD = "clear"
	


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = 13 #this may change, depending on what port the OS gives the microcontroller
s.baudrate = 9600      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)


#initalize the thrusters
X_HBRIDGE_ADDR = 0x78
Y_HBRIDGE_ADDR = 0x74
Z_HBRIDGE_ADDR = 0x73


thruster_X_A = Thruster(X_HBRIDGE_ADDR, 1, s)
thruster_X_B = Thruster(X_HBRIDGE_ADDR, 2, s)
thruster_Y_A = Thruster(Y_HBRIDGE_ADDR, 1, s)
thruster_Y_B = Thruster(Y_HBRIDGE_ADDR, 2, s)
thruster_Z_A = Thruster(Z_HBRIDGE_ADDR, 1, s)
thruster_Z_B = Thruster(Z_HBRIDGE_ADDR, 2, s)


#clear the screen
os.system(CLEAR_CMD)

magnitude = 0
direction = 1


#Main reading loop
while 1 :

	time.sleep(.2)
	
	thruster_X_A.send(magnitude*direction)
	thruster_X_B.send(magnitude*direction)
	thruster_Y_A.send(magnitude*direction)
	thruster_Y_B.send(magnitude*direction)
	thruster_Z_A.send(magnitude*direction)
	thruster_Z_B.send(magnitude*direction)
	
	#clear the screen
	os.system(CLEAR_CMD)
	print "Magnitude: " + str(magnitude) + '\n' \
		  "Direction: " + str(direction) + '\n'
	
	magnitude += 1
	if (magnitude > 127) :
		magnitude = 0
		direction = -1*direction	
	
#end of reading while loop 
	
	

#close the serial port
s.close()

