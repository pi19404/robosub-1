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

        def run(self):
            while True:
                time.sleep(update_frequency)
                communicator._refresh()

    def __init__(self, module_name, mode='debug'):
        self.module_name = module_name
        comm_json_path = os.path.join(
                os.path.abspath('../..'), 'communication_settings.json')
        self.settings = json.load(open(comm_json_path, 'r'))

        # Prepare our publisher
        self.publisher = {}
        self.publisher['context'] = zmq.Context(1)
        self.publisher['context'].bind(
                self.settings[module_name]['port'][mode])

        # Prepare our subscribers
        self.subscribers = {mname: {} for mname in self.listening()}
        for key, val in self.subscribers.items():
            val['context'] = zmq.Context(1)
            val['context'].bind(self.settings[key]['port'][mode])
            val['queue'] = []

        # Start a new thread that will continually update our message queues.
        self._refresher = Communicator._Refresher(communicator=self,
                                                  update_frequency=0.01)
        self._refresher.start()

    def _refresh(self):
        for key, val in self.subscribers.items():
            while True:
                message = None
                try:
                    message = val['context'].recv_json(zmq.DONTWAIT)
                except zmq.core.error.ZMQError:
                    break

                if message:
                    val.append(message)
                    mb_len = 'message_buffer_length'
                    if len(val['queue']) > self.settings[mb_len]:
                        val['queue'] = val['queue'][-self.settings[mb_len]:]

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

