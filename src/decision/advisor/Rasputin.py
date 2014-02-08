# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""
RoboSub Movement Program
"""

# TODO
# Maintain depth check, control power to motors, Add ability to incremental
# power increase/decrease, improve the shut off function

import argparse
import math
import os
import sys
import time
from copy import deepcopy
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator


class _Getch:
	"""Gets a single character from standard input.

	Does not echo to the screen.

	Source: ("http://code.activestate.com/recipes/"
			 "134892-getch-like-unbuffered-character-reading-from-stdin/")

	Liscense: PSF
	Created by Danny Yoo on Fri, 21 Jun 2002

	"""
	def __init__(self):
		self.impl = _GetchUnix()

	def __call__(self):
		return self.impl()


class _GetchUnix:
	def __init__(self):
		import tty, sys

	def __call__(self):
		import sys, tty, termios
		fd = sys.stdin.fileno()
		old_settings = termios.tcgetattr(fd)
		try:
			tty.setraw(sys.stdin.fileno())
			ch = sys.stdin.read(1)
		finally:
			termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
		return ch


getch = _Getch()

class AdvisorsPeon():
	def __init__(self, communicator):
		self.y = 0
		self.z = 0

		self.com = communicator

		self.missive = {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0}, "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0}}

	def decision(self, advice):
		self.missive = self.reset_missive ()

		self.missive['desired_offset']['y'] = self.y 
		self.missive['desired_offset']['z'] = self.z
		
		if advice["command"] == "forward":
			if self.y < 0:
				self.y = 0.0
			self.y += .1

			self.missive['desired_offset']['y'] = self.y 

		elif advice["command"] == "yaw left":
			self.missive['desired_orientation']['yaw'] = 3 * math.pi / 2

		elif advice["command"] == "backward":
			if self.y > 0:
				self.y = 0.0
			self.y -= .1

			self.missive['desired_offset']['y'] = self.y 

		elif advice["command"] == "yaw right":
			self.missive['desired_orientation']['yaw'] = math.pi / 2

		elif advice["command"] == "rise":
			if self.z < 0:
				self.z = 0.0
			self.z += .1

			self.missive['desired_offset']['z'] = self.z 

		elif advice["command"] == "descend":
			if self.z > 0:
				self.z = 0.0
			self.z -= .1

			self.missive['desired_offset']['z'] = self.z 

		elif advice["command"] == "stop":
			self.y = 0
			self.z = 0
			self.missive = self.reset_missive ()

		elif advice["command"] == "roll left":
			self.missive['desired_orientation']['roll'] = 3 * math.pi / 2

		elif advice["command"] == "roll right":
			self.missive['desired_orientation']['roll'] = math.pi / 2

		self.com.publish_message (self.missive)

	def reset_missive (self):
		return {"desired_offset": {"x": 0.0, "y": 0.0, "z": 0.0}, "desired_orientation": {"yaw": 0.0, "pitch": 0.0, "roll": 0.0}}


def main(args):
	print """Usage:
   +-------+-------+-------+-------+
   | 1	   | 2	   | 3	   | 4	   |
   |	   |	   |	   |	   |
   |KEYBOARD PATH  | DEPTH | N/A   |
   ++------++------++------++------++
	| q		| w		| e		| r		|
	|		|		|		|		|
	| QUIT	|FORWARD| STOP	| RISE	|
	++------++------++------++------++
	 | a	 | s	 | d	 | f	 |
	 | ROTATE|		 | ROTATE|		 |
	 | LEFT  |	BACK | RIGHT | FALL  |
	 +-------+-------+-------+-------+
"""
	com = Communicator(
			module_name=args.module_name,
			settings_path=args.settings_path)

	advice_template = {"command": None}
	
	advisor = AdvisorsPeon (com)

	while True:
		key = getch()
		print "{0}\r".format(key),
		advice = deepcopy(advice_template)

		if key == 'w':
			advice["command"] = "forward"
		elif key == 'a':
			advice["command"] = "yaw left"
		elif key == 's':
			advice["command"] = "backward"
		elif key == 'd':
			advice["command"] = "yaw right"
		elif key == 'r':
			advice["command"] = "rise"
		elif key == 'f':
			advice["command"] = "descend"
		elif key == 't':
			advice["command"] = "roll left"
		elif key == 'g':
			advice["command"] = "roll right"
		elif key == 'e':
			advice["command"] = "stop"
		elif key == '1':
			advice["command"] = "state: keyboard"
		elif key == '2':
			advice["command"] = "state: path"
		elif key == '3':
			advice["command"] = "state: depth"
	   #elif key == '6':
	   #	advice["command"] =
	   #elif key == '7':
	   #	advice["command"] =
	   #elif key == '8':
	   #	advice["command"] =
	   #elif key == '9':
	   #	advice["command"] =
		elif key == 'q':
			sys.exit()

		advisor.decision (advice)

def commandline():
	parser = argparse.ArgumentParser(description='Keyboard advice module.')
	parser.add_argument(
			'--settings_path', type=str,
			default=None,
			help='Settings file path.')
	parser.add_argument(
			'-m', '--module_name', type=str,
			default='decision/advisor',
			help='Module name.')
	return parser.parse_args()

if __name__ == '__main__':
	args = commandline()
	main(args)

