# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
import serial
from random import random
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

def main():
    accel_com = Communicator(module_name='sensor/serial/accelerometer')
    gyro_com = Communicator(module_name='sensor/serial/gyroscope')
    compass_com = Communicator(module_name='sensor/serial/compass')

    ser = serial.Serial(baudrate=56818, port=2, timeout=None)
    ser.open()

    mbuf = []
    while True:
        mchar = ser.read(size=1)
        if mchar == '\n':
            # We finished reading a message, so let's put it together and
            # prepare for the next message.
            message = ''.join(mbuf)
            mbuf = []
            if message.startswith("accelerometer enum"):
                accel_com.publish_message({"data": message})
            elif message.startswith("gyroscope enum"):
                gyro_com.publish_message({"data": message})
            elif message.startswith("compass enum"):
                compass_com.publish_message({"data": message})
        else:
            mbuf.append(mchar)


if __name__ == '__main__':
    main()

