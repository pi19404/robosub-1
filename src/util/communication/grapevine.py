# COPYRIGHT: Robosub Club of the Palouse under the GPL v3

import json
import os
import sys
import threading
import time
import zmq

class Communicator():
    class _Refresher(threading.Thread):
        def __init__(self, communicator, update_frequency):
            threading.Thread.__init__(self)
            while True:
                time.sleep(update_frequency)
                communicator._refresh()

    def __init__(self, module_name):

        self.module_name = module_name
        comm_json_path = os.path.join(
                os.path.abspath('../..'), 'communication_settings.json')
        self.settings = json.load(open(comm_json_path, 'r'))

        self.message_queues = {mname: [] for mname in self.listening()}
        self._refresher = Communicator._Refresher(communicator=self,
                                                  update_frequency=0.01)
        self._refresher.start()

    def _refresh(self):
        for mname in self.message_queues.keys():
            mqueue = self.message_queues[mname]
            while True:
                message = None
                try:
                    # Don't block while waiting for a message. Raises an
                    # exception on failure.
                    # TODO: what subscriber? Not yet defined.
                    message = subscriber.recv_json(zmq.DONTWAIT)
                except zmq.core.error.ZMQError:
                    break

                if message:
                    mqueue.append(message)
                    if len(mqueue) > self.settings['message_buffer_length']:
                        mqueue = mqueue[
                                -self.settings['message_buffer_length']:]

    def listening(self):
        """Returns a list of modules this Communicator is listening to."""
        return self.settings[self.module_name]

    def get_last_value(self, module_name):
        # TODO: This doesn't assert that messages are in order.
        if self.message_queues[module_name]:
            return self.message_queues[module_name][-1]
        else:
            return None

    def send_value(self, value):
        # TODO: Need to bind the socket and keep a message count, and send
        # the message.
        pass

