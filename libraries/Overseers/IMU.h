#ifndef __IMU_H__
#define __IMU_H__

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// Note: Local for IMU.h only!
#define LOG_STRING_LEN 128

// a configuration file with device addresses and number of certain devices
#include <Configurations/DeviceConfig.h>

// the control unit must be unique (??noCopy does not guarantee this??)
#include <Utility/NoCopy.h>

// for the Arduino communication protocols that allow communication with the
// accelerometer, compass, and gyro
#include <Wire.h>
#include <ComPort/WireComPort.h>

// for the accelerometer
#include <I2CDevices/Accel.h>

// for the compass
#include <I2CDevices/Compass.h>

// for the gyro
#include <I2CDevices/Gyro.h>

// inertial measurement unit
class IMU 
    :
    public NoCopy
{
public:
	IMU();
	
	// using the sensors (accelerometer, compass, gyro):
    // - call the "read" function, which instructs the sensor to read data
    // - call the function that will retrieve the data you want

    // Accelerometer
    virtual void getAccelAll(ACCEL_DATA *putHere);

    // Compass
    virtual void getCompassAll(COMPASS_DATA *putHere);

    // Gyro
    virtual void getGyroAll(GYRO_DATA *putHere);

    // the depth sensor reads a single value, so it reads and return in the
    // same function
    virtual uint16_t readDepth(void);

private:
    // for log strings that need to be constructed with variable data;
    char mLogString[LOG_STRING_LEN];

    // a local accelerometer instance
    accel mAccel;

    // a local compass instance
    compass mCompass;

    // a local gyro instance
    gyro mGyro;

    // the space for the depth sensor's data;
    // it is a simple analog device, so no handler is needed;
    // just read from the pin
    int16_t mDepthDataIn;
};

#endif //__IMU_H__
