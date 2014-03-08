#!/usr/bin/python

#import statements
import serial
import os
import time

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
control_byte = '\n'

ADC_DEPTH = 0x30

THRUSTER_BOW_SB 	= 0x10
THRUSTER_BOW_PORT 	= 0x11
THRUSTER_DEPTH_SB 	= 0x12
THRUSTER_DEPTH_PORT 	= 0x13
THRUSTER_STERN_SB 	= 0x14
THRUSTER_STERN_PORT 	= 0x15

#Function Definitions#########################################################################

"""
	Here we are trying to make sure we have actually found
	a control byte, so we receive several packets, then look
	at where we expect the control bytes to be. If they are not in the expected
	locastions, we wait for a new control byte and try again.

	X000X000X
	012345678
"""
def get_lock() :
	#variables for the sync loop
	current_byte = '\0'
	packet_array = ""
	in_sync = False
	
	#reset the serial port
	s.close()
	s.open()

	print "Aquiring stream sync"

	while in_sync == False:
		#read a packet from the serial port
		current_byte = s.read()

		#if the byte is the control_byte, then receive several packets
		#otherwise, we will jump back to the top of the loop and get another byte
		if current_byte == control_byte :
			packet_array = "" # clear out the array
			packet_array += current_byte  # add the byte to the array

			#receive several packets
			while len(packet_array) != 9 :
				packet_array += s.read()

			#check to see if the control byte is in the proper location in the received packets
			if (packet_array[0] == control_byte and \
				packet_array[4] == control_byte and \
				packet_array[8] == control_byte) : 
				
				#throw away rest of last packet
				s.read(3)

				#say we are in sync so we can break out of the loop
				in_sync = True
				print "sync locked"

#end get_lock()


"""
	This function reads a 4-byte packet from the serial port.
	It will also check to make sure we are still in sync, and
	pauses the program if we lose sync. It will then attempt
	to get back into sync with the serial stream.
"""
	
def get_packet() :

	success = False
	
	while success == False :

		#read 4 bytes from the serial port
		packet = s.read(4)
		
		#ensure we are in sync by checking that the control byte is in the correct place
		if packet[0] != control_byte : #if we are not in sync
			print "Error: lost sync. Press the [Enter] key to attempt to re-sync"
			raw_input() #waits for the user to press the enter key
			s.flushInput() #flushes the serial rx buffer
			get_lock() #get back into sync
		else : #if we are in sync, break out of loop
			success = True
			

	return packet
#end get_packet()


"""
	cmd_thruster() sends a thruster control command to the microncontroller
	It takes an id, and a value between +127 and -127 (negative is reverse)
"""
def cmd_thruster(thruster_id, magnitude) :

	raw_thruster_id = '\0'
	direction_mag = 0;
	raw_direction_mag = '\0'
	raw_cmd = ""

	#the chr() command converts the integer to the ascii character representation, which is a raw byte

	#convert the thruster id to a raw binary value	
	raw_thruster_id = chr(thruster_id)
	
	#make sure magnitude is within bounds
	if (magnitude > 127) :
		magnitude = 127
	elif (magnitude < -127) :
		magnitude = -127
	
	#convert direction and magnitude variable into a raw byte
	raw_magnitude = chr(magnitude & 0xFF)

	CONTROL_BYTE = '\n'
	#combine the raw bytes
	raw_cmd = CONTROL_BYTE + raw_thruster_id + raw_magnitude

	#send the commmand to the microcontroller
	s.write(raw_cmd)


#end cmd_thruster()


#stops all thrusters
def cmd_stop_all() :
	cmd_thruster(THRUSTER_BOW_SB, 0)
	cmd_thruster(THRUSTER_BOW_PORT, 0) 
	cmd_thruster(THRUSTER_STERN_SB, 0) 
	cmd_thruster(THRUSTER_STERN_PORT, 0)
	cmd_thruster(THRUSTER_DEPTH_SB, 0)
	cmd_thruster(THRUSTER_DEPTH_PORT, 0)   
#end cmd_move_forward()


def convert_depth(raw_depth) :

	depth_feet = raw_depth*0.1075 - 54.622

	return depth_feet
#end convert_depth()



#Main code####################################################################################


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = "/dev/ttyUSB0" #this may change, depending on what port the OS gives the microcontroller
s.baudrate = 56818      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)

f = open("dlog", "w")


get_lock() #get in sync with the incoming stream from the microcontroller


#Initialize some variables
ADC_DEPTH_val = -1
depth_error = 0
depth_error_integral = 0
current_depth = 0
thruster_value = 0
start_time = time.time()
wait_time = start_time + 1

	
	
#initialize the sub to stop (multiple sends to ensure packet sync occurs)
cmd_stop_all()
cmd_stop_all()
cmd_stop_all()
cmd_stop_all()


#control parameters
control_period = 1
proportion_factor = 10
integral_factor = 0

desired_depth = 7

#Main reading loop
while 1 :

	#depth control logic (PI controller)
	if (time.time() > wait_time) :
		
		#get current depth
		current_depth = convert_depth(ADC_DEPTH_val)
		
		#calculate the error
		depth_error = current_depth - desired_depth
		
		#calculate the integral of the error
		depth_error_integral = depth_error_integral + depth_error*control_period
		
		#calculate the proper correction value
		thruster_value = (proportion_factor * depth_error) + (integral_factor * depth_error_integral)
		thruster_value = int(round(thruster_value)) #convert float to int

		#send command to the trusters
		cmd_thruster(THRUSTER_DEPTH_SB, thruster_value)
		cmd_thruster(THRUSTER_DEPTH_PORT, thruster_value)  
		
		wait_time = time.time() + control_period
	#end of depth control logic
		
		
	#receive a packet
	received_packet = get_packet()
	
	#pull the device information out of the packet
	device = ord(received_packet[1]) #second byte of packet is device information (first byte is always control byte)

			
	if device == ADC_DEPTH :
		ADC_DEPTH_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		f.write("ADC_DEPTH," + str(ADC_DEPTH_val) + '\n')		
		

	os.system('clear')
	print "Depth Control Test\n"
	print "Depth: %lf feet" % ( current_depth )
	print "Desired Depth: %d feet" % (desired_depth)
	print "Error: %lf feet" % (depth_error)
	print "Integral of Error: %lf feet" % (depth_error_integral)
	print "Thruster Strength: %d" % (thruster_value)
	print "Run Time (minutes): %lf" % ((time.time() - start_time)/60)
	

	#time.sleep(0.001) #time.sleep(seconds)

#end of reading while loop 
	
	

#close the serial port
s.close()

