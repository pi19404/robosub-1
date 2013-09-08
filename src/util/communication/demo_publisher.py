import time
import zmq
from random import random

def main():
    """ main method """
    message = {
        'module': None,
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

    for _ in range(5):
        message['move']['x'] = random() * 2 - 1
        message['move']['y'] = random() * 2 - 1
        message['move']['z'] = random() * 2 - 1
        message['rotate']['x_axis'] = random() * 2 - 1
        message['rotate']['y_axis'] = random() * 2 - 1
        message['rotate']['z_axis'] = random() * 2 - 1
        message['time'] = time.time()

        publisher.send_json(message)
        time.sleep(1)

    publisher.close()
    context.term()


if __name__ == "__main__":
    main()

