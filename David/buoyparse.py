import re
import subprocess
import os
import fcntl
import traceback

matcher = re.compile('<\buoy, ((green)|(red)|(yellow)), (-?[0-9]*\\.[0-9]*), (-?[0-9]*\\.[0-9]*), (-?[0-9]*\\.[0-9]*), ([\\[\\?])>')

class thing(object):

    def __init__(self, args):
        readfd, writefd =  os.pipe()
        fl = fcntl.fcntl(readfd, fcntl.F_GETFL)
        fcntl.fcntl(readfd, fcntl.F_SETFL, fl | os.O_NONBLOCK)
        readfile = os.fdopen(readfd)
        self.proc = subprocess.Popen(args, stdout=writefd)
        self.output = []
        self.fragment = ''

    def bytes(self, s):
        for c in s:
            if c == '\n':
                self.line(self.fragment)
                self.fragment = ''
            else:
                self.fragment += c

    def line(self, s):
        match = re.match(matcher, s)
        color, x, y, z, certainty = match.group(1), match.group(5), match.group(6), match.group(7), match.group(8)
        self.output.append((color, float(x),float(y), float(z), certainty))

    def get_output(self):
        tmp = self.output
        self.output = []
        return tmp
    #def kill(self):
        

b = thing(['bash', '-c', 'while true; do echo "<buoy, green, -4.083810, -2.297143, 79.858223, [>"; done'])
try:
    while True:
        s = b.proc.stdout.read()
        b.bytes(s)
        print b.get_output()
except Exception:
	traceback.print_exc()
finally:
    b.kill()
    b.wait()
