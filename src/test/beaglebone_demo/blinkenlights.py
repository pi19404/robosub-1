import Adafruit_BBIO.GPIO as GPIO
import time

class Blinkenlights(object):
    def __init__(self, num_pins=8):
        s = 10
        e = s + num_pins + 1  # 1 to fix range
        self.pins = ["P8_" + str(idx) for idx in range(s, e)]
        for pin in self.pins:
            GPIO.setup(pin, GPIO.OUT)

    def set_low(self):
        self.set_int(0)

    def set_high(self):
        self.set_int(2 ** 16 - 1)

    def set_int(self, val):
        mask = 1
        for pin in self.pins:
            if mask & val:
                GPIO.output(pin, GPIO.HIGH)
            else:
                GPIO.output(pin, GPIO.LOW)
            mask <<= 1

    def set_gray(self, val):
        self.set_int((val >> 1) ^ val)

def rotate_right(x, n):
    return x[-n:] + x[:-n]

def pi(n):
    if n <= 1:
        return (0,)
    x = pi(n - 1) + (n - 1,)
    return rotate_right(tuple(x[k] for k in x), 1)

def p(n, j, reverse=False):
    if n == 1 and j == 0:
        if not reverse:
            yield (0,)
            yield (1,)
        else:
            yield (1,)
            yield (0,)
    elif j >= 0 and j < n:
        perm = pi(n - 1)
        if not reverse:
            for x in p(n - 1, j - 1):
                yield (1,) + tuple(x[k] for k in perm)
            for x in p(n - 1, j):
                yield (0,) + x
        else:
            for x in p(n - 1, j, reverse=True):
                yield (0,) + x
            for x in p(n - 1, j - 1, reverse=True):
                yield (1,) + tuple(x[k] for k in perm)

def monotonic(n):
    for i in range(n):
        for x in (p(n, i) if i % 2 == 0 else p(n, i, reverse=True)):
            yield x

def monotonic_int(pins):
    for val in monotonic(pins):
        yield int(''.join([str(bit) for bit in val]), base=2)


if __name__ == '__main__':
    blinken = Blinkenlights()
    for i in range(2 ** 8 - 1):
        #print i
        blinken.set_int(i)
        time.sleep(0.1)
    """
    for i in monotonic_int(15):
        #print i
        blinken.set_int(i)
    """

