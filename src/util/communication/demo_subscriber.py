# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
import zmq
import time

REFRESH_FREQUENCY = 1

def main():
    """ main method """
    # Prepare our context and publisher
    context = zmq.Context(1)
    subscriber = context.socket(zmq.SUB)
    subscriber.connect("tcp://localhost:5563")
    subscriber.setsockopt(zmq.SUBSCRIBE, '')

    while True:
        # Process all messages in the queue
        while True:
            try:
                # Don't block while waiting for a message.
                message = subscriber.recv_json(zmq.DONTWAIT)
                print(message)
            except zmq.core.error.ZMQError:
                print "No message in queue."
                break
        time.sleep(REFRESH_FREQUENCY)

    subscriber.close()
    context.term()


if __name__ == "__main__":
    main()

