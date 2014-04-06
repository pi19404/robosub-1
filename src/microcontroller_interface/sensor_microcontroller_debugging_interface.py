#!/usr/bin/python

#import statements
import serial
import os
import time

#Global Constants#############################################################################
#These values are temporary, for testing. They WILL change in the final product
#It was recommended that these values should be placed in a dictionary
control_byte = '\n'
ACL_1_X_addr = 0x10
ACL_1_Y_addr = 0x11
ACL_1_Z_addr = 0x12
GYRO_1_X_addr = 0x20
GYRO_1_Y_addr = 0x21
GYRO_1_Z_addr = 0x22

ADC_DEPTH = 0x30
ADC_BATT  = 0x31

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


#Main code####################################################################################


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = "/dev/ttyUSB0" #this may change, depending on what port the OS gives the microcontroller
s.baudrate = 56818      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)

#clear the screen
os.system('clear')

get_lock() #get in sync with the stream


#Initialize some variables
ACL_1_X_val = -1
ACL_1_Y_val = -1
ACL_1_Z_val = -1
GYRO_1_X_val = -1
GYRO_1_Y_val = -1
GYRO_1_Z_val = -1
ADC_DEPTH_val = -1
ADC_BATT_val = -1
buffer_size_max = 0
buffer_tick = 1
buffer_total = 1
sent_time = 0
received_time = 0
min_ping_time = 500
ping_tick = 1
ping_total = 1
wait_time = time.time() + 1
x_update = time.time()
x_period = 500
x_total = 1
x_tick = 1
UART_queue_len = 500
UART_queue_len_max = 0
start_time = time.time()


#Main reading loop
while 1 :
		
	#receive a packet
	received_packet = get_packet()
	
	#pull the device information out of the packet
	device = ord(received_packet[1]) #second byte of packet is device information (first byte is always control byte)

	os.system('clear')
	print "Sensor Test"
	
	#if-elif statement looks for what device the packet is concerning, and handles the data appropriately
	if device == ACL_1_X_addr :
		#pulls the data out the last two bytes of the packet
		ACL_1_X_val = int(( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 ))

		
		#data is stored in 2's complement form, this does the appropriate conversion
		if ACL_1_X_val > 32767 :
			ACL_1_X_val = (ACL_1_X_val-65536)
			
	
	elif device == ACL_1_Y_addr :
		ACL_1_Y_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if ACL_1_Y_val > 32767 :
			ACL_1_Y_val = (ACL_1_Y_val-65536)
			

	elif device == ACL_1_Z_addr :
		ACL_1_Z_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if ACL_1_Z_val > 32767 :
			ACL_1_Z_val = (ACL_1_Z_val-65536)
			
			
	elif device == GYRO_1_X_addr :
		GYRO_1_X_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if GYRO_1_X_val > 32767 :
			GYRO_1_X_val = (GYRO_1_X_val-65536)
	
	elif device == GYRO_1_Y_addr :
		GYRO_1_Y_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if GYRO_1_Y_val > 32767 :
			GYRO_1_Y_val = (GYRO_1_Y_val-65536)
			
	elif device == GYRO_1_Z_addr :
		GYRO_1_Z_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		if GYRO_1_Z_val > 32767 :
			GYRO_1_Z_val = (GYRO_1_Z_val-65536)
			
	elif device == ADC_DEPTH :
		ADC_DEPTH_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		
	elif device == ADC_BATT :
		ADC_BATT_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )
		

	print "ACL X: %d" % (ACL_1_X_val)
	print "ACL Y: %d" % (ACL_1_Y_val)
	print "ACL Z: %d" % (ACL_1_Z_val)
	print "GYRO X: %d" % (GYRO_1_X_val)
	print "GYRO Y: %d" % (GYRO_1_Y_val)
	print "GYRO Z: %d" % (GYRO_1_Z_val)
	print "ADC Depth: %d" % ((ADC_DEPTH_val) )
	print "ADC Battery: %lf" % ((ADC_BATT_val) * 3.3/1024 * 7.5)
	
	print "buffer size: %d" % (s.inWaiting())
	print "Run Time (minutes): %lf" % ((time.time() - start_time)/60)
	
	
#end of reading while loop 
	
	

#close the serial port
s.close()

