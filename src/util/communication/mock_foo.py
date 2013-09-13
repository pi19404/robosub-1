import time
import grapevine
from random import random

UPDATE_FREQUENCY = 0.3

if __name__ == '__main__':
    com = grapevine.Communicator('util/communication/mock_foo')

    message = {
        'module': None,
        'message_number': None,
        'time': None,
        'move': {
            'x': None,
            'y': None,
            'z': None},
        'rotate': {
            'x_axis': None,
            'y_axis': None,
            'z_axis': None}}

    while True:
        message['move']['x'] = random() * 2 - 1
        message['move']['y'] = random() * 2 - 1
        message['move']['z'] = random() * 2 - 1
        message['rotate']['x_axis'] = random() * 2 - 1
        message['rotate']['y_axis'] = random() * 2 - 1
        message['rotate']['z_axis'] = random() * 2 - 1
        com.send_message(message)
        time.sleep(UPDATE_FREQUENCY)

