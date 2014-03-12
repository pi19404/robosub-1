#!/usr/bin/env python
import os
import sys
import zmq
import numpy as np
from time import time
from threading import Thread, Event
from multiprocessing.pool import ThreadPool
sys.path.append(os.path.abspath('../../..'))
from robosub_settings import settings

class Streamer(object):
    """Stream robosub video.

    Also allow client to choose which stage of video processing should be
    streamed.

    """

    def __init__(self, fp, module_name):
        """Initialize video stream server.

        Args:

            fp - FrameProcessor object that provides image processing tools for
            robosub vision modules.

            module_name - Name of the module that this Streamer plugged into.

        """
        # Frame Processing tool managed by Stream Processor.
        self._fp = fp
        self._port_span = settings[module_name]['port_span']
        self._context = zmq.Context(1)
        # Set the function that chooses which image to send.
        self._image_chooser = lambda fp : fp.im
        # Image gets split horizontally into 2**x images and stored as a list
        # of image parts. Sender thread sends image parts over 2**x sockets.
        self._im_parts = None
        # Socket connections for sending each image part. One socket per part.
        self._sockets = []
        # Control socket is the first socket in the port range. Listens for
        # client commands.
        self._control_socket = self._context.socket(zmq.PAIR)
        # Allow connections to socket on all interfaces.
        self._control_socket.bind("tcp://*:{port}".format(
                port=settings[module_name]['stream_port']))
        # Build the rest of the sockets. These are for sending image parts.
        for port_offset in xrange(1, self._port_span):
            self._sockets += [self._context.socket(zmq.PAIR)]
            self._sockets[-1].bind("tcp://*:{port}".format(
                    port=settings[module_name]['stream_port'] + port_offset))

        # Parent thread sets busy to tell sender thread to get busy. Sender
        # thread clears busy when it's done sending.
        self._busy = Event()

        # Sender thread splits the image into parts and sends the image.
        self._sender_thread = Thread(target=self._sender)
        self._sender_thread.daemon = True
        self._sender_thread.start()

        # Command listener thread listens for client commands and changes which
        # images are streamed based on those commands.
        self._command_listener_thread = Thread(target=self._command_listener)
        self._command_listener_thread.daemon = True
        self._command_listener_thread.start()

    def _sender(self):
        """Wait for busy signal and then send the current image to the client.

        This should be run as a thread.

        """
        p = ThreadPool(processes=self._port_span)
        while True:
            # Wait until we're told to get busy.
            self._busy.wait()
            self._last_successful_send = time()
            print self._last_successful_send
            # FIXME if we ever change the resolution to something that can't be
            # split evenly into this many parts, this will break. Possibly
            # check for this and bitwise or the image with a bigger image when
            # necessary?
            self._im_parts = np.split(
                    ary=self._image_chooser(self._fp),
                    indices_or_sections=self._port_span - 1)
            # Map each image part to a pool worker and wait until all the
            # workers finish sending their image parts.
            p.map(func=self._worker_send,
                    iterable=range(self._port_span - 1))
            self._busy.clear()

    def _command_listener(self):
        """Listen for commands from the client.

        Client commands tell which stage of image processing should be
        streamed.

        Remarks - This method should be run as a thread.

        """
        while True:
            command_packet = self._control_socket.recv_json()
            # Check for a keyboard command.
            try:
                # Change the image choosing function we use.
                self._image_chooser = \
                        settings['sensor/vision/plugin/Streamer'] \
                        ['command_map'][command_packet['command']]['func']
            except KeyError:
                pass

            # Check if this is a confirmation that the last frame was received
            # and clear the busy event if it is. Wait for this confirmation
            # because we don't want to queue frames up in the socket. It's
            # better to skip frames.
            try:
                if command_packet['timestamp'] >= self._last_successful_send:
                    print 'clearing the busy flag'
                    # All done, let the parent know we can handle the next image!
                    self._busy.set()
            except KeyError:
                pass

            # Client can also specifically ask for more data. Prevents
            # deadlocks where Streamer thinks it sent a packet, but client
            # never confirms it. Instead of waiting forever, the client asks
            # for the next packet when nothing is in the socket.
            try:
                if 'send_more' in command_packet:
                    print 'client asking for more'
                    self._busy.clear()
            except KeyError:
                pass

    def _worker_send(self, idx):
        """Send the image part given at given idx over socket at given idx.

        """
        # Build the important information about how to reconstruct this image.
        metadata = dict(image_part=idx,
                dtype = str(self._fp.im.dtype),
                shape = self._im_parts[idx].shape,
                timestamp = self._last_successful_send)
        try:
            # Send metadata, but let the receiver know we're sending more stuff
            # that's part of this packet.
            self._sockets[idx].send_json(
                    metadata, flags=zmq.SNDMORE | zmq.NOBLOCK)
            # Second part of the packet, the actual image.
            self._sockets[idx].send(
                    self._im_parts[idx], copy=True, track=False, flags=zmq.NOBLOCK)
        except zmq.ZMQError:
            self._last_successful_send = 0.0
            # Nobody connected? That's okay.
            pass

    def process_image(self, packet):
        """Process the image that is currently loaded in the FrameProcessor.

        If not already busy, stream a single image from the FrameProcessor.

        Remarks - This function is required function for all StreamProcessor
        plugins.

        """
        # Make sure that sender isn't still busy with the last image. If it is,
        # don't worry about sending this one.
        if not self._busy.isSet():
            # Tell the waiting sender thread to get busy!
            self._busy.set()

if __name__ == '__main__':
    pass
    # TODO make this runnable by itself? Too much work?
