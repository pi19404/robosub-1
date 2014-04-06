#!/usr/bin/env python
#import statements
import serial
import os
import time
import math
##Make sure correct imports
import argparse
import sys
from copy import deepcopy
from random import random
sys.path.append(os.path.abspath(".."))
from util.communication.grapevine import Communicator

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
CONTROL_BYTE = '\n'

LED_CONTROL_0 = 0x30

#Function Definitions#########################################################################

com = Communicator(module_name="microcontroller/debug_lights")
def cmd_LED(pattern) :

    last_packet_time = 0

    raw_id = '\0'
    raw_pattern = '\0'
    raw_cmd = ""

    #the chr() command converts the integer to the ascii character representation, which is a raw byte

    #convert the LED id to a raw binary value	
    raw_id = chr(LED_CONTROL_0)

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
s.port = "/dev/ttyUSB0" #this will vary, depending on what port the OS gives the microcontroller
s.baudrate = 56818      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)


#Initialize some variables
wait_time = time.time()
patternNumber = 3

#Initialize pattern variables
#set up list 

#Main reading loop
#directionary index of incoming_packet
last_packet_time = 0.0
while True:
    incoming_packet = com.get_last_message("sensor/vision/cam_down")
    if incoming_packet and incoming_packet['timestamp'] > last_packet_time:
        print 'got a packet'
        last_packet_time = incoming_packet['timestamp']
        if "VisionPath" in incoming_packet:
            if incoming_packet["VisionPath"]["angle2"] is not None:
                if (abs(incoming_packet["VisionPath"]["angle1"]) < (math.pi / 16)) or (abs(incoming_packet["VisionPath"]["angle2"]) < (math.pi / 16)):
                    print "Robosub sees two paths and is aligned with one of them!"
                    cmd_LED(2) #Needs to be GREEN LED
                else:
                    print "Robosub sees two paths and is not aligned with them!"
                    cmd_LED(4) #Needs to be PURPLE LED
            else:
                if (abs(incoming_packet["VisionPath"]["angle1"]) < (math.pi / 16)):
                    print "Robosub sees 1 path and is aligned with it!"
                    cmd_LED(3) #Needs to be BLUE LED
                else:
                    print "Robosub sees 1 path and is not aligned with it!"
                    cmd_LED(5) #Needs to be ORANGE LED
        else:
            print "Robosub does not see any path!"
            cmd_LED(1) #Needs to be RED LED
    time.sleep(.01)   
#end of reading while loop 
        
        

#close the serial port
s.close()

