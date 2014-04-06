# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""Sanitation Module

This module subscribes to all the sensors and uses the sensor data to create
a organized human-readable python dictionary for use throughout the system.

"""

# TODO: I ended up stripping out the base_data calibration. This should be
# worked back in. The best place to put it is in robosub_settings.py.

from abc import ABCMeta, abstractmethod
import argparse
import os
import sys
import threading
import time
sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import Communicator


class Sensor(threading.Thread):
    __metaclass__ = ABCMeta

    def __init__(self, calibration_samples, com_module_name, epoch):
        threading.Thread.__init__(self)
        self.daemon = True
        self.calibration_samples = calibration_samples
        self.com = Communicator(module_name=com_module_name)
        self.calibrated = False
        self.epoch = epoch

    def run(self):
        while not self.calibrated:
            self.com.debug("Calibrating {0}...".format(type(self)))
            self.calibrate(self.calibration_samples)
            self.sleep()

        while True:
            self.com.publish_message(self.fetch_data())
            self.com.debug('sleeping')
            self.sleep()

    @abstractmethod
    def fetch_data(self):
        pass

    @abstractmethod
    def calibrate(self, samples=10):
        pass

    def sleep(self):
        time.sleep(self.epoch)


class Gyroscope(Sensor):
    def __init__(self, calibration_samples, epoch):
        Sensor.__init__(
                self, calibration_samples, "datafeed/sanitized/gyroscope",
                epoch)
        self.gx = None
        self.gy = None
        self.gz = None

    def __str__(self):
        return ("Gyroscope -- gx: {self.gx} gy: {self.gy} gz: {self.gy}"
                "".format(self=self))

    def fetch_data(self):
        data = {}
        # First get all Gyroscope and Accelerometer messages since they come
        # seperately
        data = {
            'gx': None,
            'gy': None,
            'gz': None
        }

        while True:
            data_msg = self.com.get_last_message("datafeed/raw/gyroscope")
            print data_msg
            if data_msg:
                if data['gx'] is None:
                    data['gx'] = data_msg.get('GYRO_X')

                if data['gy'] is None:
                    data['gy'] = data_msg.get('GYRO_Y')

                if data['gz'] is None:
                    data['gz'] = data_msg.get('GYRO_Z')
            if None not in data.values():
                break
            self.sleep()

        return data

    def calibrate(self, samples=10):
        self.gx = 0.0
        self.gy = 0.0
        self.gz = 0.0

        for _ in xrange(samples):
            data = self.fetch_data()
            self.gx += data['gx']
            self.gy += data['gy']
            self.gz += data['gz']

        self.gx /= samples
        self.gy /= samples
        self.gz /= samples

        self.calibrated = True


class Accelerometer(Sensor):
    def __init__(self, calibration_samples, epoch):
        Sensor.__init__(
                self, calibration_samples, "datafeed/sanitized/accelerometer",
                epoch)
        self.calibration_samples = calibration_samples
        self.ax = None
        self.ay = None
        self.az = None

    def __str__(self):
        return ("Accelerometer -- ax: {self.ax} gy: {self.ay} gz: {self.ay}"
                "".format(self=self))

    def fetch_data(self):
        accel = {
            'ax': None,
            'ay': None,
            'az': None
        }

        while True:
            accel_msg = self.com.get_last_message("datafeed/raw/accelerometer")

            if accel_msg:
                if accel['ax'] is None:
                    accel['ax'] = accel_msg.get('ACL_X')

                if accel['ay'] is None:
                    accel['ay'] = accel_msg.get('ACL_Y')

                if accel['az'] is None:
                    accel['az'] = accel_msg.get('ACL_Z')

                if accel['ax'] is None or accel['ay'] is None or accel['az'] is None:
                    continue
                else:
					#print accel
					break
            self.sleep()
		
        return accel

    def calibrate(self, samples=10):
        self.ax = 0.0
        self.ay = 0.0
        self.az = 0.0

        for _ in xrange(samples):
            data = self.fetch_data()
            self.ax += data['ax']
            self.ay += data['ay']
            self.az += data['az']

        self.ax /= samples
        self.ay /= samples
        self.az /= samples

        self.calibrated = True


class BatteryVoltage(Sensor):
    def __init__(self, calibration_samples, epoch):
        Sensor.__init__(
                self, calibration_samples,
                "datafeed/sanitized/battery_voltage", epoch)
        self.calibration_samples = calibration_samples
        self.voltage = None

    def __str__(self):
        return "BatteryVoltage -- voltage: {0}".format(self.voltage)

    def fetch_data(self):
        battery = {'voltage': None}
        batt_msg = None
        while not batt_msg:
            batt_msg = self.com.get_last_message(
                    "datafeed/raw/battery_voltage")
            if not batt_msg:
                self.sleep()

        batt_raw = batt_msg.get('BATTERY_VOLTAGE')
        if batt_raw is None:
            self.com.debug('Error getting battery value')
        else:
            battery['voltage'] = batt_raw

        return battery

    def calibrate(self, samples=10):
        self.voltage = 0.0
        for _ in xrange(samples):
            self.voltage += self.fetch_data()['voltage']
        self.voltage /= samples
        self.calibrated = True


class Depth(Sensor):
    def __init__(self, calibration_samples, epoch):
        Sensor.__init__(
                self, calibration_samples, "datafeed/sanitized/depth", epoch)
        self.calibration_samples = calibration_samples
        self.depth = None

    def __str__(self):
        return "Depth -- depth: {0}".format(self.depth)

    def fetch_data(self):
        depth_msg = None
        while not depth_msg:
            depth_msg = self.com.get_last_message("datafeed/raw/depth")
            if not depth_msg:
                self.sleep()
        depth_raw = depth_msg.get('DEPTH')
        self.com.debug("depth_raw", depth_raw)
        depth = {'value': None}
        if depth_raw is None:
            self.com.debug('Error getting depth value')
        else:
            # XXX: Why these numbers?
            depth['value'] = 0.1075 * float(depth_raw) - 54.622
        return depth

    def calibrate(self, samples=10):
        self.depth = 0.0
        for _ in xrange(samples):
            self.depth += self.fetch_data()['value']
        self.depth /= samples
        self.calibrated = True


class Compass(Sensor):
    def __init__(self, calibration_samples, epoch):
        Sensor.__init__(
                self, calibration_samples, "datafeed/sanitized/compass", epoch)
        self.cx = None
        self.cy = None
        self.cz = None
        self.calibration_samples = calibration_samples

    def __str__(self):
        return "TODO: Compass"

    def fetch_data(self):
        compass = {
            'cx': None,
            'cy': None,
            'cz': None
        }

        while True:
            comp_msg = self.com.get_last_message("datafeed/raw/compass")
            if comp_msg:
                if compass['cx'] is None:
                    compass['cx'] = comp_msg.get('MAG_X')

                if compass['cy'] is None:
                    compass['cy'] = comp_msg.get('MAG_Y')

                if compass['cz'] is None:
                    compass['cz'] = comp_msg.get('MAG_Z')

                if compass['cx'] is None or compass['cy'] is None or compass['cz'] is None:
                    continue
                else:
                    break
            self.sleep()
        return compass

    def calibrate(self, samples=10):
        self.cx = 0.0
        self.cy = 0.0
        self.cz = 0.0

        for _ in xrange(samples):
            data = self.fetch_data()
            self.cx += data['cx']
            self.cy += data['cy']
            self.cz += data['cz']

        self.cx /= samples
        self.cy /= samples
        self.cz /= samples

        self.calibrated = True


def commandline():
    parser = argparse.ArgumentParser(description='Sanitization module.')
    parser.add_argument(
            '-e', '--epoch', type=float, default=0.01667,
            help='Sleep time per cycle.')
    parser.add_argument('-d', '--debug', action="store_true", default=False)
    parser.add_argument('--calibration_samples', type=int, default=10)
    return parser.parse_args()

def main():
    args = commandline()

    gyro = Gyroscope(
            calibration_samples=args.calibration_samples, epoch=args.epoch)
    gyro.start()

    accel = Accelerometer(
            calibration_samples=args.calibration_samples, epoch=args.epoch)
    accel.start()

    volt = BatteryVoltage(
            calibration_samples=args.calibration_samples, epoch=1)
    volt.start()

    depth = Depth(
            calibration_samples=args.calibration_samples, epoch=0.05)
    depth.start()

    compass = Compass(
            calibration_samples=args.calibration_samples, epoch=args.epoch)
    compass.start()

    # This thread really has nothing to do.
    while True:
        time.sleep(100)


if __name__ == '__main__':
    main()

