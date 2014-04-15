#!/usr/bin/python

#import statements
import serial
import os
import time

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
CONTROL_BYTE = '\n'

LED_CONTROL_0 = 0x30
LED_CONTROL_1 = 0x31



#Function Definitions#########################################################################

def cmd_LED(pattern, id) :

	raw_id = '\0'
	raw_pattern = '\0'
	raw_cmd = ""

	#the chr() command converts the integer to the ascii character representation, which is a raw byte

	#convert the LED id to a raw binary value	
	raw_id = chr(id)
	
	#convert pattern to raw binary value
	raw_pattern = chr(pattern)

	#combine the raw bytes
	raw_cmd = CONTROL_BYTE + raw_id + raw_pattern

	#send the commmand to the microcontroller
	s.write(raw_cmd)


#end cmd_LED()


#Main code####################################################################################


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = 14 #this will vary, depending on what port the OS gives the microcontroller
s.baudrate = 9600      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)


#Initialize some variables
wait_time = time.time()
patternNumber = 3

#Main reading loop
while 1 :
	if (time.time() > wait_time) :
		print patternNumber
		cmd_LED(patternNumber, LED_CONTROL_0)
		cmd_LED(patternNumber, LED_CONTROL_1)
		wait_time = time.time() + .5
		patternNumber = patternNumber + 1
		if(patternNumber > 5):
			patternNumber = 0
	else :
		time.sleep(.1)
			
#end of reading while loop 
	
	

#close the serial port
s.close()

