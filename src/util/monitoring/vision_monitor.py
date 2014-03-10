#!/usr/bin/env python

# FIXME not all of these are needed.
import sys
import os
import zmq
#import json
import cv2
import Tkinter as tk
#import cv2.cv as cv
import argparse
from time import sleep, time
from threading import Event, Thread
from multiprocessing.pool import ThreadPool
import numpy as np
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator
from robosub_settings import settings

class VisionViewer(object):
    def __init__(self, addr, camera):
        self._module_name = 'sensor/vision/cam_{direction}'.format(direction=camera)
        self._port_span = settings[self._module_name]['port_span']
        self._context = zmq.Context(1)
        self._sockets = []
        self._control_socket = self._context.socket(zmq.PAIR)
        self._control_socket.connect(
                "tcp://{addr}:{port}".format(
                addr=addr, port=settings[self._module_name]['stream_port']))
        for port_offset in xrange(1, self._port_span):
            self._sockets += \
                [self._context.socket(zmq.PAIR)]
            self._sockets[-1].connect(
                    "tcp://{addr}:{port}".format(
                    addr=addr, port=settings[self._module_name]['stream_port'] + port_offset))
        self._im_parts = [None] * (self._port_span - 1)
        self._im = None
        self._busy = Event()

        self._receiver_thread = Thread(target=self._receiver)
        self._receiver_thread.daemon = True
        self._receiver_thread.start()

        # Build user input interface for quickly changing stream content.
        self._command_interface()

    def _command_interface(self):
        # Build Tkinter keypress interface.
        def onKeyPress(event):
            # Send the keypress over the command socket. If it's valid, the
            # streamer will change the type of image it streams.
            self._control_socket.send_json({'command':str(event.char)[0]}, zmq.NOBLOCK)
        root = tk.Tk()
        root.bind('<KeyPress>', onKeyPress)
        # Just listen for keypresses until the window closes.
        root.mainloop()

    def _worker_receive(self, idx):
        metadata = self._sockets[idx].recv_json()
        message = self._sockets[idx].recv(
                    copy=True, track=False)
        buf = buffer(message)
        image = np.frombuffer(buf, dtype=metadata['dtype'])
        self._im_parts[idx] = image.reshape(metadata['shape'])

    def _receiver(self):
        p = ThreadPool(processes=self._port_span - 1)
        while True:
            print 'hi'
            start = time()
            p.map(func=self._worker_receive,
                    iterable=range(self._port_span - 1))
            print 'bye'
            try:
                self._im = np.vstack(self._im_parts)
                print time() - start
                cv2.imshow('vision_monitor', self._im)
                cv2.waitKey(1)
            except Exception:
                # Lazy
                print "I'm too lazy"
                pass
            # FIXME parse input from the user
            self._busy.clear()

def commandline():
    parser = argparse.ArgumentParser(description="Vision Monitor")
    parser.add_argument('-i', '--ip', type=str, default='127.0.0.1',
            help='IP of computer hosting robosub video.')
    parser.add_argument('-c', '--camera', type=str,
            help='down, front, left, or right camera')
    return parser.parse_args()


if __name__ == '__main__':
    args = commandline()
    viewer = VisionViewer(args.ip, args.camera)
