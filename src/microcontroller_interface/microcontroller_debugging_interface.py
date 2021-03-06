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

THRUSTER_BOW_SB 	= 0x10
THRUSTER_BOW_PORT 	= 0x11
THRUSTER_DEPTH_SB 	= 0x12
THRUSTER_DEPTH_PORT 	= 0x13
THRUSTER_STERN_SB 	= 0x14
THRUSTER_STERN_PORT 	= 0x15

mag = 127

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


#here are some example functions controlling the thrusters for movement

#causes the sub to move forward
def cmd_move_forward() :
	cmd_thruster(THRUSTER_BOW_SB, -mag)
	cmd_thruster(THRUSTER_BOW_PORT, mag) 
	cmd_thruster(THRUSTER_STERN_SB, mag) 
	cmd_thruster(THRUSTER_STERN_PORT, -mag)  


#end cmd_move_forward()


#causes the sub to move backwards
def cmd_move_backward() :
	cmd_thruster(THRUSTER_BOW_SB, mag)
	cmd_thruster(THRUSTER_BOW_PORT, -mag) 
	cmd_thruster(THRUSTER_STERN_SB, -mag) 
	cmd_thruster(THRUSTER_STERN_PORT, mag)  
#end cmd_move_forward()


#causes the sub to dive
def cmd_dive() :
	cmd_thruster(THRUSTER_DEPTH_SB, mag)
	cmd_thruster(THRUSTER_DEPTH_PORT, mag) 
#end cmd_move_forward()


#causes the sub to surface
def cmd_surface() :
	cmd_thruster(THRUSTER_DEPTH_SB, -mag)
	cmd_thruster(THRUSTER_DEPTH_PORT, -mag) 
#end cmd_move_forward()



#causes the sub to rotate clockwise
def cmd_rotate_cw() :
	cmd_thruster(THRUSTER_BOW_SB, mag)
	cmd_thruster(THRUSTER_BOW_PORT, mag) 
	cmd_thruster(THRUSTER_STERN_SB, -mag) 
	cmd_thruster(THRUSTER_STERN_PORT, -mag)  
#end cmd_rotate_cw()



#causes the sub to rotate counter-clockwise
def cmd_rotate_ccw() :
	cmd_thruster(THRUSTER_BOW_SB, -mag)
	cmd_thruster(THRUSTER_BOW_PORT, -mag) 
	cmd_thruster(THRUSTER_STERN_SB, mag) 
	cmd_thruster(THRUSTER_STERN_PORT, mag)  

#end cmd_rotate_ccw()



#stops the depth control thrusters
def cmd_stop_depth() :
	cmd_thruster(THRUSTER_DEPTH_SB, 0)
	cmd_thruster(THRUSTER_DEPTH_PORT, 0) 
#end cmd_move_forward()	


#stops all thrusters
def cmd_stop_all() :
	cmd_thruster(THRUSTER_BOW_SB, 0)
	cmd_thruster(THRUSTER_BOW_PORT, 0) 
	cmd_thruster(THRUSTER_STERN_SB, 0) 
	cmd_thruster(THRUSTER_STERN_PORT, 0)
	cmd_thruster(THRUSTER_DEPTH_SB, 0)
	cmd_thruster(THRUSTER_DEPTH_PORT, 0)   
#end cmd_move_forward()



#Main code####################################################################################


#initialize the serial port
s = serial.Serial()	#get instance of serial class
s.port = "/dev/ttyUSB0" #this may change, depending on what port the OS gives the microcontroller
s.baudrate = 56818      #the baudrate may change in the future
s.open()		#attempt to open the serial port (there is no guard code, I'm assuming this does not fail)

f = open("slog", "w")

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

	


cmd_stop_all()
cmd_stop_all()
cmd_stop_all()
cmd_stop_all()
"""
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_move_forward()	
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_move_backward()
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_dive() 
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_surface()
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_rotate_cw()
time.sleep(5)
cmd_stop_all()
time.sleep(.1)
cmd_rotate_ccw()
time.sleep(5)
cmd_stop_all()
"""

ACL_1_X_val_old = 0
flipflop = 0

#Main reading loop
while 1 :

	#cmd_dive()
	#cmd_move_forward() 
	if (time.time() > wait_time) :
		#cmd_dive()

		if flipflop == 0 :
			cmd_move_forward()
			cmd_dive()
		else :
			cmd_move_backward()
			cmd_surface()
		
		flipflop = ~flipflop
		#cmd_move_forward() 
		"""
		cmd_move_forward()	
		cmd_thruster(THRUSTER_BOW_SB, mag, 1)
		cmd_thruster(THRUSTER_STERN_SB, mag, 0) 

		cmd_thruster(THRUSTER_BOW_PORT, mag, 0) 
		cmd_thruster(THRUSTER_STERN_PORT, mag, 1) 
		
		cmd_dive() 
		cmd_stop_all()
		
		cmd_move_forward()		
		cmd_dive()
		cmd_move_backward()
		cmd_rotate_cw()
		cmd_stop_depth()
		cmd_surface()	
		cmd_thruster(THRUSTER_BOW_SB, 100, 0)	
		cmd_thruster(THRUSTER_STERN_SB, 25, 0) 
		cmd_thruster(THRUSTER_BOW_SB, 0, 0)
		cmd_thruster(THRUSTER_STERN_SB, 0, 0) 
		cmd_stop_all()
		"""
		sent_time = time.time()
		wait_time = sent_time + .5
		
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
			
		f.write("X," + str(ACL_1_X_val) + '\n')
	
	elif device == ACL_1_Y_addr :
		ACL_1_Y_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if ACL_1_Y_val > 32767 :
			ACL_1_Y_val = (ACL_1_Y_val-65536)
			
		f.write("Y," + str(ACL_1_Y_val) + '\n')

	elif device == ACL_1_Z_addr :
		ACL_1_Z_val = ( ord(received_packet[2]) ) | \
		( ord(received_packet[3]) << 8 )

		if ACL_1_Z_val > 32767 :
			ACL_1_Z_val = (ACL_1_Z_val-65536)
			
		f.write("Z," + str(ACL_1_Z_val) + '\n')
			
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
		
			
	elif device == ord('P') :
		received_time = time.time()
		if ping_tick > 20 :
			ping_total /= 2
			ping_tick /= 2
			
		ping_total += received_time - sent_time
		ping_tick += 1
		

	print "ACL X: %d" % (ACL_1_X_val)
	print "ACL Y: %d" % (ACL_1_Y_val)
	print "ACL Z: %d" % (ACL_1_Z_val)
	print "GYRO X: %d" % (GYRO_1_X_val)
	print "GYRO Y: %d" % (GYRO_1_Y_val)
	print "GYRO Z: %d" % (GYRO_1_Z_val)
	print "ADC Depth: %d" % ((ADC_DEPTH_val) )
	print "ADC Battery: %lf" % ((ADC_BATT_val) * 3.3/1024 * 7.5)
	print "Average Ping Time: %lf" % (ping_total/ping_tick)
	
	print "buffer size: %d" % (s.inWaiting())
	print "Run Time (minutes): %lf" % ((time.time() - start_time)/60)
	
	
	if ACL_1_X_val_old == ACL_1_X_val :
		samecount = samecount + 1
	else :
		samecount = 0

	print "samecount: %d" % (samecount)

	if samecount >= 500 :
		print "FROZEN"
		samecount = 0
		raw_input()
		#s.close()
		#time.sleep(.5)
		#s.open()
		
		

	
	
	ACL_1_X_val_old = ACL_1_X_val
	

	#time.sleep(0.001) #time.sleep(seconds)

#end of reading while loop 
	
	

#close the serial port
s.close()

