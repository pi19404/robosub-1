import sys
import serial
import struct
import threading

TORPEDO_1 = 1 << 0
TORPEDO_2 = 1 << 1
MARKER_1 = 1 << 2
MARKER_2 = 1 << 3
CLAW_1_GRIP = 1 << 4
CLAW_1_ROTATE = 1 << 5
CLAW_2_GRIP = 1 << 6
RESET = 1 << 7

DOWNWARD_CLAW = 0
FORWARD_CLAW = 1

VERTICAL = 2
HORIZONTAL = 3

GRASP = 4
RELEASE = 5

class Arduino(object):

    def __init__(self, dev):
        self.ser = serial.Serial(dev,
                                 9600,
                                 timeout=None)
        self.work = False
        self.worker = None
        self.reset_flag = False

        self.armed = threading.Event()
        self.disarmed = threading.Event()

        self._reset()

    def _reset(self):
        # output
        self.x = 0
        self.y = 0
        self.z = 0
        self.torpedo_1 = False
        self.torpedo_2 = False
        self.marker_1 = False
        self.marker_2 = False
        self.claw_1_grip = False
        self.claw_1_rotate = False
        self.claw_2_grip = False
        self.delta_heading = 0.0

        # input
        self.armed.clear()
        self.disarmed.clear()

        self.sub_heading = 0.0
        self.depth = 0.0


    def _status_byte(self):
        r = 0
        if self.torpedo_1:
            r |= TORPEDO_1
        if self.torpedo_2:
            r |= TORPEDO_2
        if self.marker_1:
            r |= MARKER_1
        if self.marker_2:
            r |= MARKER_2
        if self.claw_1_grip:
            r |= CLAW_1_GRIP
        if self.claw_2_grip:
            r |= CLAW_2_GRIP
        return r

    def reset(self):
        self.reset_flag = True
        self._reset()

    def fire_torpedo_one(self):
        self.torpedo_1 = True

    def fire_torpedo_two(self):
        self.torpedo_2 = True

    def drop_marker_one(self):
        self.marker_1 = True

    def drop_marker_two(self):
        self.marker_2 = True


    def move(self, x, y, z):
        '''
        Move the sub to a new location as an offset of the current location

        x, y, z, and in meters
        '''
        self.x = x
        self.y = y
        self.z = z

    def heading_offset(self, r):
        '''
        Change the heading of the sub as an offset of the current heading

        r is in radians
        '''
        self.delta_offset = r

    def command_string(self, status_byte, x, y, z, delta_heading):
        # convert from meters to milimeters
        x, y, z, delta_heading = map(lambda n: int(n*1000.0), [x,y,z,delta_heading])

        # pack into seven bytes
        return struct.pack( '>Bhhhh',
                           status_byte,
                           x, y, z,
                           delta_heading )
    def read(self, s):
        assert( len(s) == 5 )
        armed, heading, depth = struct.unpack('>BHH', s)
        if armed != 0:
            self.armed.set()
            self.disarmed.clear()
        else:
            self.disarmed.set()
            self.armed.clear()
        self.sub_heading = heading
        self.depth = depth

    def go(self):
        self.work = True
        def work():
            n = 0
            while self.work:
                r = self.ser.read(5)
                assert(len(r) == 5)
                self.read(r)
                n += 1
                if self.reset_flag:
                    self._reset()
                    status = self._status_byte() | RESET
                else:
                    status = self._status_byte()
                output = self.command_string(status, self.x, self.y, self.z, self.delta_heading)
                #print repr(output)
                self.ser.write(output)
        self.worker = threading.Thread(target=work)
        self.worker.start()

    def stop(self):
        self.work = False

    def wait_for_armed(self, armed):
        if armed:
            self.armed.wait()
        else:
            self.disarmed.wait()

if __name__ == '__main__':
    a = Arduino(sys.argv[1])
