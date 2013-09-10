# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
import time
import zmq
from random import random

UPDATE_FREQUENCY = 0.3

def main():
    """ main method """
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

    # Prepare our context and publisher
    context = zmq.Context(1)
    publisher = context.socket(zmq.PUB)
    publisher.bind("tcp://*:5563")
    message['module'] = 'util/communication/demo_publisher'
    message_number = 1

    while True:
        message_number += 1
        message['message_number'] = message_number
        message['move']['x'] = random() * 2 - 1
        message['move']['y'] = random() * 2 - 1
        message['move']['z'] = random() * 2 - 1
        message['rotate']['x_axis'] = random() * 2 - 1
        message['rotate']['y_axis'] = random() * 2 - 1
        message['rotate']['z_axis'] = random() * 2 - 1
        message['time'] = time.time()

        publisher.send_json(message)
        print "Sent message {0}.".format(message_number)
        time.sleep(UPDATE_FREQUENCY)

    publisher.close()
    context.term()


if __name__ == "__main__":
    main()

