# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

"""This defines the python communication interface.

Initialization settings are found in robosub/src/communication_settings.json

"""

import Queue
import json
import os
import sys
import threading
import time
import zmq

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

    def __init__(self, module_name, mode=None,
                 subscriber_buffer_length=1024,
                 subscriber_high_water_mark=1024, comm_json_path=None):
        """
        'module_name' must follow the folder path name convention that
            specifies a module.
        'mode' must be 'debug' or 'release'.
        'subscriber_buffer_length' and 'subscriber_high_water_mark' control zmq
            memory settings.
        'comm_json_path' specifies an alternative (testing) communication
            settings file.

        """
        self.module_name = module_name
        if not comm_json_path:
            up_dir = lambda path: os.path.split(path)[0]
            comm_json_path = os.path.join(
                    up_dir(up_dir(up_dir(os.path.abspath(__file__)))),
                    'communication_settings.json')
        self.settings = json.load(open(comm_json_path, 'r'))
        if not mode:
            self.mode = self.settings['mode']

        # Make sure a location for sockets exists.
        if self.mode == 'debug':
            if not os.path.isdir('/tmp/robosub'):
                os.mkdir('/tmp/robosub')

        # Prepare our publisher
        self.publisher = {}
        self.publisher['next_message_number'] = 1
        self.publisher['context'] = zmq.Context(1)
        self.publisher['socket'] = self.publisher['context'].socket(zmq.PUB)
        self.publisher['socket'].hwm = \
                self.settings['publisher_high_water_mark']
        self.publisher['socket'].setsockopt(
                zmq.SNDBUF, self.settings['publisher_buffer_length'])
        self.publisher['socket'].bind(self.get_socket_name(module_name))
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
            mdata['socket'].connect(self.get_socket_name(mname))
            mdata['queue'] = Queue.Queue()
            mdata['refresh_lock'] = threading.Semaphore(value=1)

        # Set up the passive refresher
        self._refresher = Communicator._Refresher(communicator=self)
        self._refresher.daemon = True
        self._refresher.start()

    def get_last_message(self, module_name):
        """Reads all messages in the queue and returns the last one.

        The messages prior to the last one will be discarded.

        """
        try:
            return [msg for msg in self.get_messages(module_name)][-1]
        except IndexError:
            return None

    def get_messages(self, module_name):
        """Returns a generator that yields all available messages."""
        self._refresh(module_name)
        while True:
            try:
                yield self.subscribers[module_name]['queue'].get_nowait()
            except Queue.Empty:
                raise StopIteration()

    def get_socket_name(self, module_name):
        """Determines the socket for module_name."""
        if self.mode == 'debug':
            return "ipc:///tmp/robosub/{port}.ipc".format(
                    port=self.settings[module_name]['port'])
        elif self.mode == 'release':
            return "tcp://{ip}:{port}".format(
                    ip=self.settings[module_name]['ip'],
                    port=self.settings[module_name]['port'])
        else:
            assert False, "mode '{0}' not recognized".format(self.mode)

    def listening(self):
        """Returns a list of modules this Communicator is listening to."""
        listening_to = self.settings[self.module_name].get('listen')
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
        self.publisher['socket'].send_json(message)

    def _refresh(self, module_name):
        """Reads the zmq socket and stores all messages into a queue."""
        subscriber_data = self.subscribers.get(module_name)
        if not subscriber_data:
            assert False, "'{0}' not recognized".format(module_name)

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

