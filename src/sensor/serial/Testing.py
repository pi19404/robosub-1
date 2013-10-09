# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
import serial
from random import random
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

class SerialCom(Communicator)


def main():
    accel_com = Communicator(module_name='sensor/serial/accelerometer')
    gyro_com = Communicator(module_name='sensor/serial/gyroscope')
    compass_com = Communicator(module_name='sensor/serial/compass')

    ser = serial.Serial(); #open up an instance of the serial module
    ser.baudrate = 56818 #set baud rate
    ser.port = 2 #changes depending on what port we're using
    ser.open() #opens the serial port

    i = 0
    while True:



        i += 1
        message['move']['x'] = random() * 2 - 1
        com.publish_message(message)
        print 'sent', i
        time.sleep(UPDATE_FREQUENCY)
        recv_msg = com.get_last_message('util/communication/test/mock_bar')
        if recv_msg:
            print recv_msg

        time.sleep(EPOCH)



    #this code just reads a newline-terminated line and prints it to the display
    mstring = ""
    mchar  = '0'
    while 1:
        mchar = ser.read();
        if mchar != '\n' :
            mstring += mchar
        else:
            print mstring
            mstring = ""



if __name__ == '__main__':
    main()








ser = serial.Serial(); #open up an instance of the serial module
ser.baudrate = 56818 #set baud rate
ser.port = 2 #changes depending on what port we're using
ser.open() #opens the serial port
#ser.read() #reads one byte
#ser.write('a') #writes one byte



#this code just reads a newline-terminated line and prints it to the display
mstring = ""
mchar  = '0'
while 1:
    mchar = ser.read();
    if mchar != '\n' :
        mstring += mchar
    else:
        print mstring
        mstring = ""
        
