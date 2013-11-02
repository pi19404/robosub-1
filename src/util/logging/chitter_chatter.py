from subprocess import check_output
import Queue
import Tkinter as tk
import argparse
import json
import pprint
import threading
import time

class GuiPart(object):
    """
    This recipe describes how to handle asynchronous I/O in an environment where
    you are running tk as the graphical user interface. tk is safe
    to use as long as all the graphics commands are handled in a single thread.
    Since it is more efficient to make I/O channels to block and wait for something
    to happen rather than poll at regular intervals, we want I/O to be handled
    in separate threads. These can communicate in a threasafe way with the main,
    GUI-oriented process through one or several queues. In this solution the GUI
    still has to make a poll at a reasonable interval, to check if there is
    something in the queue that needs processing. Other solutions are possible,
    but they add a lot of complexity to the application.

    Copyright (PSF): Jacob Hallen, AB Strakt, Sweden. 2001-10-17
    """
    def __init__(self, master, queue, endCommand):
        self.queue = queue
        # Set up the GUI
        console = tk.Button(master, text='Done', command=endCommand)
        console.pack()

        watch_frame = tk.Frame(master)
        watch_frame.pack(fill=tk.BOTH, expand=True)
        self.watch_window = tk.Text(watch_frame)
        self.watch_window.pack(fill=tk.BOTH, expand=True)

        # Add more GUI stuff here

    def processIncoming(self):
        """
        Handle all the messages currently in the queue (if any).
        """
        while self.queue.qsize():
            try:
                msg = self.queue.get(0)
                self.watch_window.delete(0.0, tk.END)

                for line in pprint.pformat(msg, width=1).splitlines():
                    line = line.encode('ascii')
                    line.strip()
                    line = line.replace(r'\n', '\n')
                    self.watch_window.insert(tk.END, line + '\n')
                #self.watch_window.insert(tk.END, pprint.pformat(msg, width=1))
            except Queue.Empty:
                pass

class ThreadedClient(object):
    """
    Launch the main part of the GUI and the worker thread. periodicCall and
    endApplication could reside in the GUI part, but putting them here
    means that you have all the thread controls in a single place.

    Copyright (PSF): Jacob Hallen

    """
    def __init__(self, master, args):
        """
        Start the GUI and the asynchronous threads. We are in the main
        (original) thread of the application, which will later be used by
        the GUI. We spawn a new thread for the worker.
        """
        self.master = master

        self.module = args.module
        self.epoch = args.epoch
        self.log_path = args.log_path
        self.quiet = args.quiet

        # Create the queue
        self.queue = Queue.Queue()

        # Set up the GUI part
        self.gui = GuiPart(master, self.queue, self.endApplication)

        # Set up the thread to do asynchronous I/O
        # More can be made if necessary
        self.running = 1
        self.thread1 = threading.Thread(target=self.workerThread1)
        self.thread1.start()

        # Start the periodic call in the GUI to check if the queue contains
        # anything
        self.periodicCall()

    def periodicCall(self):
        """
        Check every 100 ms if there is something new in the queue.
        """
        self.gui.processIncoming()
        if not self.running:
            # This is the brutal stop of the system. You may want to do
            # some cleanup before actually shutting it down.
            import sys
            sys.exit(1)
        self.master.after(100, self.periodicCall)

    def workerThread1(self):
        """
        This is where we handle the asynchronous I/O. For example, it may be
        a 'select()'.
        One important thing to remember is that the thread has to yield
        control.
        """
        with open(self.log_path, 'r') as fin:
            last_msg = {'message_number': 0}
            while self.running:
                try:
                    msg = json.loads(fin.readline())
                except ValueError:
                    # This fails for a few reasons. The main reason is that
                    # the msg is '' and the error will be "No JSON object
                    # could be decoded". At least one more source of a
                    # ValueError exists.
                    time.sleep(self.epoch)
                    continue
                if (msg and
                    msg['module_name'] == self.module and
                    msg['message_number'] > last_msg['message_number']):
                    last_msg = msg
                    self.queue.put(pprint.pformat(msg, width=1))
                    if not self.quiet:
                        print msg
                else:
                    pass

    def endApplication(self):
        self.running = 0


def commandline():
    parser = argparse.ArgumentParser(description='Mock module.')
    parser.add_argument('-m', '--module', type=str,
            help='Module to watch.')
    parser.add_argument('-l', '--log_path', type=str,
            default='/tmp/robosub/log.out',
            help='Module to watch.')
    parser.add_argument('-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('-q', '--quiet', action='store_true',
            help='Suppress printing to stdout.')
    return parser.parse_args()


if __name__ == '__main__':
    root = tk.Tk()

    client = ThreadedClient(root, args=commandline())
    root.mainloop()

