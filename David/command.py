import struct

def message(header, body = ''):
    size_header = struct.pack('<ii', len(header) + len(body), len(header))
    return size_header + header + body

CLAW_COMMAND = {
        "EXTEND": 0,
        "RETRACT": 1,
        "GRASP": 2,
        "RELEASE": 3,
        }


class Arduino:
    '''
    Send commands to the arduino.
    '''

    def __init__(self, f):
        '''
        Takes an open file to write commands to

        Should be the serial port connected to the arduino
        '''
        self.f = f

    def reset(self):
        self.f.write(message('RESET'))

    def fire_torpedo_one(self):
        self.f.write(message('TRPDO_1'))

    def fire_torpedo_two(self):
        self.f.write(message('TRPDO_2'))

    def drop_marker_one(self):
        self.f.write(message('MARKER_1'))

    def drop_marker_two(self):
        self.f.write(message('MARKER_2'))

    def claw(self, claw, command):
        assert(claw in ['FORWARD_CLAW', 'DOWNWARD_CLAW'])
        body = struct.pack('b', CLAW_COMMAND[command])
        self.f.write(message(claw, body))

    def move(self, x, y, z):
        '''
        Move the sub to a new location as an offset of the current location

        x, y, z, and in meters
        '''
        self.f.write(message('POS_OFFSET',"%s,%s,%s" % (x,y,z)))

    def heading_offset(self, r):
        '''
        Change the heading of the sub as an offset of the current heading

        r is in radians
        '''
        self.f.write(message('HEADING_OFFSET', str(r)))


if __name__ == '__main__':
    import sys
    a = Arduino(sys.stdout)
    a.reset()
    a.fire_torpedo_one()
    a.fire_torpedo_two()
    a.drop_marker_one()
    a.drop_marker_two()
    a.drop_marker_two()
    for claw in ['FORWARD_CLAW', 'DOWNWARD_CLAW']:
        for command in ['EXTEND', 'RETRACT', 'GRASP', 'RELEASE']:
            a.claw(claw, command)
    a.move(1.1,2.2,3.3);
    a.heading_offset(6.28);

