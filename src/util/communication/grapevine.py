# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""This defines the python communication interface.

This assumes that robosub_settings.py is on PYTHONPATH.

"""

import Queue
import os
import threading
import time
import zmq
from robosub_settings import settings
from numpy import array, frombuffer

def get_socket_name(module_name, socket_type='pub'):
    """Determines the socket for module_name."""
    try:
        ip = settings[module_name]['ip']
    except KeyError:
        # FIXME this isn't the best solution for pub vs sub.
        if socket_type == 'pub':
            ip = '*'
        else:
            ip = '127.0.0.1'
    return 'tcp://{ip}:{port}'.format(ip=ip, port=settings[module_name]['port'])

class Communicator(object):
    """The robosub communication interface."""
    class _Refresher(threading.Thread):
        """This passively reads messages from the sockets."""

        # While the high water mark provides a similar functionality,
        # it will cause the system to drop new messages. In certain scenarios,
        # the only messages received would be stale. In order to avoid
        # this situation, the _Refresher will passively populate a local
        # queue.
        # Note: if update_frequency isn't high enough, this system can still
        # drop fresh messages.

        def __init__(self, communicator, update_frequency=0.01):
            threading.Thread.__init__(self)
            self.communicator = communicator
            self.update_frequency = update_frequency

        def run(self):
            while True:
                for module_name in self.communicator.listening():
                    self.communicator._refresh(module_name)
                time.sleep(self.update_frequency)

    def __init__(self, module_name,
                 subscriber_buffer_length=1024,
                 subscriber_high_water_mark=1024):
        """
        'module_name' must follow the folder path name convention that
            specifies a module.
        'subscriber_buffer_length' and 'subscriber_high_water_mark' control zmq
            memory settings.

        """
        self.settings = settings
        self.module_name = module_name
        self.module_name_debug = os.path.join(self.module_name, "debug")

        if not os.path.isdir('/tmp/robosub'):
            os.mkdir('/tmp/robosub')

        # Prepare our publisher
        self.publisher = {}
        self.publisher['next_message_number'] = 1
        self.publisher['context'] = zmq.Context(1)
        self.publisher['socket'] = self.publisher['context'].socket(zmq.PUB)
        self.publisher['socket'].hwm = settings['publisher_high_water_mark']
        self.publisher['socket'].setsockopt(
                zmq.SNDBUF, settings['publisher_buffer_length'])
        self.publisher['socket'].bind(get_socket_name(self.module_name))

        # Prepare the debug channel
        self.publisher_debug = {}
        self.publisher_debug['next_message_number'] = 1
        self.publisher_debug['context'] = zmq.Context(1)
        self.publisher_debug['socket'] = \
                self.publisher_debug['context'].socket(zmq.PUB)
        self.publisher_debug['socket'].hwm = \
                settings['publisher_high_water_mark']
        self.publisher_debug['socket'].setsockopt(
                zmq.SNDBUF, settings['publisher_buffer_length'])
        self.publisher_debug['socket'].bind(
                get_socket_name(self.module_name_debug))

        # Note: Even though the bind call returns, the socket isn't actually
        # ready for some short amount of time after this call. zmq will
        # drop messages that are published in this time.

        # Prepare our subscribers
        self.subscribers = {}
        for mname in self.listening():
            self.subscribers[mname] = {}
            mdata = self.subscribers[mname]
            mdata['context'] = zmq.Context(1)
            mdata['socket'] = mdata['context'].socket(zmq.SUB)
            mdata['socket'].setsockopt(zmq.SUBSCRIBE, '')
            mdata['socket'].setsockopt(zmq.RCVBUF, subscriber_buffer_length)
            mdata['socket'].hwm = subscriber_high_water_mark
            mdata['socket'].connect(get_socket_name(mname, 'sub'))
            mdata['queue'] = Queue.Queue()
            mdata['refresh_lock'] = threading.Semaphore(value=1)
            mdata['last_message'] = None

        # Set up the passive refresher
        self._refresher = Communicator._Refresher(communicator=self)
        self._refresher.daemon = True
        self._refresher.start()

    def get_last_message(self, module_name):
        """Reads all messages in the queue and returns the last one.

        The messages prior to the last one will be discarded. If no
        new messages were received since the last call to this method,
        this will return the last message again.

        """
        # Updates the last message
        list(self.get_messages(module_name))
        return self.subscribers[module_name]['last_message']

    def get_messages(self, module_name):
        """Returns a generator that yields all available messages."""
        self._refresh(module_name)
        while True:
            try:
                msg = self.subscribers[module_name]['queue'].get_nowait()
                if msg:
                    self.subscribers[module_name]['last_message'] = msg
                yield msg
            except Queue.Empty:
                raise StopIteration()

    def bind_video_stream(self, port, n_ports=30):
        """Bind one end of a socket pair for video streaming."""
        # FIXME change the dictionary structure for stream socket pair.
        # Maybe make an entirely new dictionary.
        self.publisher['stream'] = {}
        self.publisher['stream']['context'] = zmq.Context(1)
        self.publisher['stream']['socket'] = \
                    self.publisher['stream']['context'].socket(zmq.PAIR)

        for i in xrange(n_ports):
            self.publisher['stream']['socket'].bind(
                        "tcp://*:{port}".format(port=port + i))
        #self.poller = zmq.Poller()
        #self.poller.register(self.publisher['stream']['socket'], zmq.POLLOUT)

    def connect_video_stream(self, port, addr='127.0.0.1'):
        """Connect one end of a socket pair for video streaming."""
        self.subscribers['stream'] = {}
        self.subscribers['stream']['context'] = zmq.Context(1)
        self.subscribers['stream']['socket'] = \
                    self.subscribers['stream']['context'].socket(zmq.PAIR)
        self.subscribers['stream']['socket'].connect(
                    "tcp://{addr}:{port}".format(addr=addr, port=port))

    def send_image(self, image):
        """Send an image over the connected stream server socket"""

        metadata = dict(dtype = str(image.dtype), shape = image.shape)
        try:
            self.publisher['stream']['socket'].send_json(
                    metadata, flags=zmq.SNDMORE | zmq.NOBLOCK)
            self.publisher['stream']['socket'].send(
                    image, copy=True, track=False, flags=zmq.NOBLOCK)
        except zmq.ZMQError:
            pass

    def recv_image(self):
        """Receive an image from the connected stream client socket."""
        metadata = self.subscribers['stream']['socket'].recv_json()
        message = self.subscribers['stream']['socket'].recv(
                    copy=True, track=False)
        buf = buffer(message)
        image = frombuffer(buf, dtype=metadata['dtype'])
        return image.reshape(metadata['shape'])

    def listening(self):
        """Returns a list of modules this Communicator is listening to."""
        listening_to = settings[self.module_name].get('listen')
        if not listening_to:
            listening_to = []
        return listening_to

    def publish_message(self, message):
        """Publishes a message to all listeners.

        Messages should be in the form of a dict. The 'message_number' and
        'time' fields will automatically be updated. If the message is not
        a dict, this will create a dict and store the message under the key
        'message'.

        """
        if not isinstance(message, dict):
            message = {"message": message}
        message['message_number'] = self.publisher['next_message_number']
        self.publisher['next_message_number'] += 1
        message['timestamp'] = time.time()
        message['module_name'] = self.module_name
        self.publisher['socket'].send_json(message)

    def debug(self, *args):
        message = {}
        message['message'] = ' '.join([str(x) for x in args])
        message['message_number'] = self.publisher_debug['next_message_number']
        self.publisher_debug['next_message_number'] += 1
        message['timestamp'] = time.time()
        message['module_name'] = self.module_name_debug
        self.publisher_debug['socket'].send_json(message)

    def _refresh(self, module_name):
        """Reads the zmq socket and stores all messages into a queue."""
        subscriber_data = self.subscribers.get(module_name)
        if not subscriber_data:
            raise KeyError(
                    "Module '{0}' is not listening to '{1}'."
                    "".format(self.module_name, module_name))

        while True:
            try:
                # This lock is needed because there are two potential
                # writers to this queue. While the Queue object
                # can probably handle it, we don't want the messages
                # to be inserted out of order. The scope of this lock
                # could be made finer by having a separate lock for
                with subscriber_data['refresh_lock']:
                    subscriber_data['queue'].put_nowait(
                            subscriber_data['socket'].recv_json(zmq.DONTWAIT))
            except zmq.core.error.ZMQError:
                break

