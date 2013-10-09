# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import time
import os
import sys
import serial
from random import random
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator

EPOCH = 0.01

def main():
    accel_com = Communicator(module_name='sensor/serial/accelerometer')
    gyro_com = Communicator(module_name='sensor/serial/gyroscope')
    compass_com = Communicator(module_name='sensor/serial/compass')

    # The timeout makes reads immediately return. We will need to count the
    # characters to see if we successfully read anything.
    ser = serial.Serial(baudrate=56818, port=2, timeout=0)
    ser.open()

    mbuf = []
    while True:
        while True:
            mchar = ser.read(size=1)
            if not mchar:
                # No data, so let's do something else.
                break
            elif mchar == '\n':
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

        time.sleep(EPOCH)


if __name__ == '__main__':
    main()

