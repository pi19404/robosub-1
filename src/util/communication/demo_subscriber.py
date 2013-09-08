import zmq
import time

def main():
    """ main method """

    # Prepare our context and publisher
    context    = zmq.Context(1)
    subscriber = context.socket(zmq.SUB)
    subscriber.connect("tcp://localhost:5563")
    subscriber.setsockopt(zmq.SUBSCRIBE, '')

    start = time.time()
    # TODO: This timer thing doesn't work. Use threads.
    while time.time() - start < 10:
        message = subscriber.recv_json()
        print(message)

    subscriber.close()
    context.term()


if __name__ == "__main__":
    main()
