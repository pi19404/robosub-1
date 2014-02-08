# COPYRIGHT: Robosub Club of the Palouse under the GPL v3
"""Sanitation Module (sanitation.py)

This module subscribes to all the sensors and uses the sensor data to create
a organized human-readable python dictionary for use throughout the system.

"""

from abc import ABCMeta, abstractmethod
import json
import zmq
import os
import sys
import datetime
import threading
from util.communication.grapevine import Communicator

DEBUG = False

def dprint(*args):
    if DEBUG:
        print ' '.join([str(arg) for arg in args])

class Sensor(threading.Thread):
    __metaclass__ = ABCMeta

    def __init__(self, calibration_samples, com_in, com_out):
        threading.Thread.__init__(self)
        self.calibration_samples = calibration_samples
        self.com_in = com_in
        self.com_out = com_out
        self.calibrated = False

    @abstractmethod
    def run(self):
        while not self.calibrated:
            dprint("Calibrating {0}...".format(type(self))
            self.calibrate()
        while True:
            self.com_out.publish_message(self.fetch_data())

    @abstractmethod
    def fetch_data(self):
        pass

    @abstractmethod
    def calibrate(self, samples=10):
        pass

class Gyroscope(Sensor):
    def __init__(self, calibration_samples, com_in, com_out):
        Sensor.__init__(self, com_in, com_out)
        self.calibration_samples = calibration_samples
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
            data_msg = san.get_last_message("sensor/datascope")
            if data_msg:
                if data['gx'] is None:
                    data['gx'] = data_msg.get('GYRO_X')

                if data['gy'] is None:
                    data['gy'] = data_msg.get('GYRO_Y')

                if data['gz'] is None:
                    data['gz'] = data_msg.get('GYRO_Z')

            if data['gx'] is None or data['gy'] is None or data['gz'] is None:
                continue
            else:
                # if we have data for all axises then adjust for calibration
                # and break;
                if baseData != None:
                    data['gx'] -= baseData['gyroscope']['gx']
                    data['gy'] -= baseData['gyroscope']['gy']
                    data['gz'] -= baseData['gyroscope']['gz']
                break

        return data

    def calibrate(self, samples=10):
        self.gx = 0.0
        self.gy = 0.0
        self.gz = 0.0

        for _ in xrange(samples):
            data = self.fetch_data()
            gx += data['gx']
            gy += data['gy']
            gz += data['gz']

        self.gx /= samples
        self.gy /= samples
        self.gz /= samples

        self.calibrated = True

class Accelerometer(Sensor):
    def __init__(self, calibration_samples, com_in, com_out):
        Sensor.__init__(self, communicator)
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
            accel_msg = san.get_last_message("sensor/accelerometer")
            if accel_msg:
                if accel['ax'] is None:
                    accel['ax'] = accel_msg.get('ACL_X')

                if accel['ay'] is None:
                    accel['ay'] = accel_msg.get('ACL_Y')

                if accel['az'] is None:
                    accel['az'] = accel_msg.get('ACL_Z')

                if (accel['ax'] is None or
                    accel['ay'] is None or
                    accel['az'] is None):
                    continue
                else:
                    # if we have data for all axises then break;
                    if baseData != None:
                        accel['ax'] -= baseData['accelerometer']['ax']
                        accel['ay'] -= baseData['accelerometer']['ay']
                        accel['az'] -= baseData['accelerometer']['az']
                    break

        obj['accelerometer'] = accel

    def calibrate(self, samples=10):
        self.ax = 0.0
        self.ay = 0.0
        self.az = 0.0

        for _ in xrange(samples):
            data = self.fetch_data()
            gx += data['gx']
            gy += data['gy']
            gz += data['gz']

        self.ax /= samples
        self.ay /= samples
        self.az /= samples

        self.calibrated = True

class BatteryVoltage(Sensor):
    def __init__(self, calibration_samples, com_in, com_out):
        Sensor.__init__(self, communicator)
        self.calibration_samples = calibration_samples
        self.voltage = None

    def __str__(self):
        return "BatteryVoltage -- voltage: {0}".format(self.voltage)

    def fetch_data(self):
        batt_msg = san.get_last_message("sensor/battery_voltage")
        if batt_msg:
            batt_raw = batt_msg.get('BATTERY_VOLTAGE')
            if batt_raw is None:
                dprint('Error getting battery value')
            else:
                battery = {'voltage' : batt_raw}

                obj['battery_voltage'] = battery

    def calibrate(self, samples):
        self.voltage = 0.0
        for _ in xrange(samples):
            self.voltage += self.fetch_data()['battery_voltage']
        self.voltage /= samples
        self.calibrated = True

class Depth(Sensor):
    def __init__(self, calibration_samples, com_in, com_out):
        Sensor.__init__(self, communicator)
        self.calibration_samples = calibration_samples
        self.depth = None

    def __str__(self):
        return "Depth -- depth: {0}".format(self.depth)

    def fetch_data(self):
        # Now lets get the depth using the conversion function
        # XXX: Why these numbers?
        # y = 0.1075x - 54.622

        depth_msg = san.get_last_message("sensor/depth")
        if depth_msg:
            depth_raw = depth_msg.get('DEPTH')
            if depth_raw is None:
                dprint('Error getting depth value')
            else:
                depth_actual = 0.1075 * depth_raw - 54.622

                if baseData != None:
                    depth_actual -= baseData['depth']['value']

                depth =  {'value' : depth_actual}

                obj['depth'] = depth

    def calibrate(self, samples):
        self.depth = 0.0
        for _ in xrange(samples)
            self.depth += self.fetch_data()['depth']
        self.depth /= samples
        self.calibrated = True


class Compass(Sensor):
    def __init__(self, calibration_samples, com_in, com_out):
        Sensor.__init__(self, communicator)
        self.calibration_samples = calibration_samples

    def __str__(self):
        return "TODO: Compass"

    def fetch_data(self):
        raise NotImplementedError()

# Example Sensor Object
# XXX This has been split up into 5 different modules.
#
# sensors = {
#     'gyroscope' : {
#         'gx' : 1,
#         'gy' : 2,
#         'gz' : 3
#     },
#     'accelerometer' : {
#         'ax' : 1,
#         'ay' : 2,
#         'az' : 3
#     },
#     'battery_voltage' :  {
#         'voltage' : 5
#     },
#     'depth' : {
#         'value' : 10
#     },
#     'compass' : {
#         'direction' : 0
#     }
# }
def fetch_data(obj, baseData=None):
#   # First get all Gyroscope and Accelerometer messages since they come seperately
#   gyro = {
#       'gx': None,
#       'gy': None,
#       'gz': None
#   }

#   while True:
#       gyro_msg = san.get_last_message("sensor/gyroscope")
#       if gyro_msg:
#           if gyro['gx'] is None:
#               gyro['gx'] = gyro_msg.get('GYRO_X')

#           if gyro['gy'] is None:
#               gyro['gy'] = gyro_msg.get('GYRO_Y')

#           if gyro['gz'] is None:
#               gyro['gz'] = gyro_msg.get('GYRO_Z')

#       if gyro['gx'] is None or gyro['gy'] is None or gyro['gz'] is None:
#           continue
#       else:
#           # if we have data for all axises then adjust for calibration and break;
#           if baseData != None:
#               gyro['gx'] -= baseData['gyroscope']['gx']
#               gyro['gy'] -= baseData['gyroscope']['gy']
#               gyro['gz'] -= baseData['gyroscope']['gz']
#           break

#   obj['gyroscope'] = gyro

    # Now accelerometer
#   accel = {
#       'ax':None,
#       'ay':None,
#       'az':None
#   }

#   while True:
#       accel_msg = san.get_last_message("sensor/accelerometer")
#       if accel_msg:
#           if accel['ax'] is None:
#               accel['ax'] = accel_msg.get('ACL_X')

#           if accel['ay'] is None:
#               accel['ay'] = accel_msg.get('ACL_Y')

#           if accel['az'] is None:
#               accel['az'] = accel_msg.get('ACL_Z')

#           if (accel['ax'] is None or
#               accel['ay'] is None or
#               accel['az'] is None):
#               continue
#           else:
#               # if we have data for all axises then break;
#               if baseData != None:
#                   accel['ax'] -= baseData['accelerometer']['ax']
#                   accel['ay'] -= baseData['accelerometer']['ay']
#                   accel['az'] -= baseData['accelerometer']['az']
#               break

#   obj['accelerometer'] = accel


    # Now lets get the depth using the conversion function
    # XXX: Why these numbers?
    # y = 0.1075x - 54.622

#   depth_msg = san.get_last_message("sensor/depth")
#   if depth_msg:
#       depth_raw = depth_msg.get('DEPTH')
#       if depth_raw is None:
#           print 'Error getting depth value'
#       else:
#           depth_actual = 0.1075 * depth_raw - 54.622

#           if baseData != None:
#               depth_actual -= baseData['depth']['value']

#           depth =  {'value' : depth_actual}

#           obj['depth'] = depth

    # And lastly, battery voltage
#   batt_msg = san.get_last_message("sensor/battery_voltage")
#   if batt_msg:
#       batt_raw = batt_msg.get('BATTERY_VOLTAGE')
#       if batt_raw is None:
#           print 'Error getting battery value'
#       else:


#           battery = {'voltage' : batt_raw }

#           obj['battery_voltage'] = battery


    # TODO: We will add compass later
    #comp_msg = san.get_last_message("sensor/compass")

def calibrate(dataSampleSize=10):
    """This will calibrate the data upon placing the sub into water.

    1) I will collect N number of data points from the Microcontroller
    2) I will then find the average of all N data points
    3) There will be a base sensors object that will act as a relative
       calibration metric to define base values for the sub

    """

    dataPool = [] #Used to store list of data objects

    percentComplete = 0
    for i in range(0 , dataSampleSize):
        dataObj = {}
        percentComplete = float(float(i) / dataSampleSize * 100)
        os.system("clear")
        dprint("Starting Calibration...")
        dprint(percentComplete, "% complete...")
        fetch_data(dataObj)
        dataPool.append(dataObj)


    # Now we have a list of N data points for each sensor


    # So lets go through each and build up the average
    averageObj = {
#       'accelerometer' : {
#           'ax': 0,
#           'ay': 0,
#           'az': 0
#       },
#       'gyroscope' : {
#           'gx': 0,
#           'gy': 0,
#           'gz': 0
#       },
#       'depth': {
#           'value': 0
#       },
#       'battery_voltage': {
#           'voltage': 0
#       },
#       'compass': {
#           'direction':0
#       }
#   }

#   ax = ay = az = gx = gy = gz = depth = voltage = 0

#   for obj in dataPool:
#       ax += obj['accelerometer']['ax']
#       ay += obj['accelerometer']['ay']
#       az += obj['accelerometer']['az']

#       gx += obj['gyroscope']['gx']
#       gy += obj['gyroscope']['gy']
#       gz += obj['gyroscope']['gz']

#       depth += obj['depth']['value']

#       voltage += obj['battery_voltage']['voltage']

#   averageObj['accelerometer']['ax'] = ax/dataSampleSize
#   averageObj['accelerometer']['ay'] = ay/dataSampleSize
#   averageObj['accelerometer']['az'] = az/dataSampleSize
#   averageObj['gyroscope']['gx'] = gx/dataSampleSize
#   averageObj['gyroscope']['gy'] = gy/dataSampleSize
#   averageObj['gyroscope']['gz'] = gz/dataSampleSize
#   averageObj['depth']['value'] = depth/dataSampleSize
#   averageObj['battery_voltage']['voltage'] = voltage/dataSampleSize
#   print
#   print "Calibration complete..."
#   os.system("sleep 1")
#   print
#   print "----------- Base sensor object ---------"
#   print averageObj
#   return averageObj

def commandline():
    parser = argparse.ArgumentParser(description='Sanitization module.')
    parser.add_argument(
            '-e', '--epoch', type=float,
            default=0.05,
            help='Sleep time per cycle.')
    parser.add_argument('--debug', action="store_true", default=False)
    parser.add_argument('--calibration_samples', type=int, default=10)
    return parser.parse_args()

def main():
    global DEBUG
    args = commandline()
    DEBUG = ags.debug
    com_in = Communicator(module_name="sensor/sanitation")

    gyro = Gyroscope(
            com_in, Communicator(module_name="sensor/sanitized/gyroscope"))
    gyro.start()

    accel = Gyroscope(
            com_in, Communicator(module_name="sensor/sanitized/accelerometer"))
    accel.start()

    volt = BatteryVoltage(
            com_in,
            Communicator(module_name="sensor/sanitized/battery_voltage"))
    volt.start()

    depth = Depth(
            com_in, Communicator(module_name="sensor/sanitized/depth"))
    depth.start()

   #compass = Compass(
   #        com_in, Communicator(module_name="sensor/sanitized/compass"))
   #compass.start()


if __name__ == '__main__':
    main()

