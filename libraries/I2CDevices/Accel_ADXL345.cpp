#ifdef HAVE_ACCEL
#if ACCEL_MODEL==ADXL345

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// for declaration of the accelerometer class
#include "Accel.h"

// for use of the Arduino "Wire" object
#include <Wire.h>

// these are the addresses for the registers that we will be using
// Note: axis registers are appended with '0' and '1' because that is the way
// that they appear in the register tables in the microchip's manual
#define ACL_BASE        0x1D
#define ACL_PWR_CTRL    0x2D
#define ACL_X0          0x32
#define ACL_X1          0x33
#define ACL_Y0          0x34
#define ACL_Y1          0x35
#define ACL_Z0          0x36
#define ACL_Z1          0x37

/*
The device should only need to be initialized once, and that is on powerup.  We 
are not handling the case where something gets unplugged and has to be 
reconnected.  If that happens, just reconnect the device and power cycle the 
microcontroller, and then things will reinitialize.
*/
accel::accel()
{
    uint8_t dataByte;

    // read the power control register, set the mode bit from standby to 
    // measure, then write back the modified register

    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_PWR_CTRL, &dataByte);
    dataByte |= 0x08;
    writeDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_PWR_CTRL, dataByte);
}

void accel::readDeviceRegister
    (uint8_t devAddr, uint8_t regAddr, uint8_t *dataByte)
{
    // start up a local instance of the log manager
    LogManager& mLogInstance = LogManager::GetInstance();

    Wire.beginTransmission(devAddr);
    Wire.send(regAddr);
    Wire.endTransmission();

    Wire.requestFrom(devAddr, (uint8_t)1);

    Wire.beginTransmission(devAddr);
    while(!Wire.available());
    while(Wire.available())
    {
        *dataByte = Wire.receive();
    }
    Wire.endTransmission();
}

void accel::writeDeviceRegister
    (uint8_t devAddr, uint8_t regAddr, uint8_t dataByte)
{
    // start up a local instance of the log manager
    LogManager& mLogInstance = LogManager::GetInstance();

    Wire.beginTransmission(devAddr);
    Wire.send(regAddr);
    Wire.send(dataByte);
    Wire.endTransmission();
}

void accel::readAllAxes(ACCEL_DATA *putDataHere)
{
    // read and return all accelerometer handler data
    uint8_t dataByte;
    int16_t localX;
    int16_t localY;
    int16_t localZ;

    // start up an instance of the log manager 
    LogManager& mLogInstance = LogManager::GetInstance();

    if (NULL == putDataHere)
    {
        mLogInstance.LogStr("accel::readAllAxes - null argument; aborting");
        putDataHere->X = 0.0;
        putDataHere->Y = 0.0;
        putDataHere->Z = 0.0;
        return;
    }

    // start up local variables
    dataByte = 0;
    localX = 0;
    localY = 0;
    localZ = 0;

    // now read the X registers; first the high, then the low
    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_X1, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read X1 = ", dataByte);
    localX = dataByte << 8;

    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_X0, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read X0 = ", dataByte);
    localX |= dataByte;
//    mLogInstance.LogStrHex("accel::readAllAxes - localX = ", localX);

    // now read the Y registers.
    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_Y1, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read Y1 = ", dataByte);
    localY = dataByte << 8;

    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_Y0, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read Y0 = ", dataByte);
    localY |= dataByte;
//    mLogInstance.LogStrHex("accel::readAllAxes - localY = ", localY);

    // now read the Z registers.
    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_Z1, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read Z1 = ", dataByte);
    localZ = dataByte << 8;

    readDeviceRegister((uint8_t)ACL_BASE, (uint8_t)ACL_Z0, &dataByte);
//    mLogInstance.LogStrHex("accel::readAllAxes - Read Z0 = ", dataByte);
    localZ |= dataByte;
//    mLogInstance.LogStrHex("accel::readAllAxes - localZ = ", localY);


    // now apply the binary -> float conversion factor and put the data into 
    // the provided data structure;
    // now apply the binary -> float conversion factor and put the data into 
    // the provided data structure
    /*
        We want our units are in g's.  For reference, Earth's gravity = 1g.

        On page 4 of the accel chip's datasheet, the sensitivity for a 
        measurement range of +-2g, 10bit resolutiondps, which is the default 
        and what we are using, is the typical value of 3.9mg/LSB.  Multiplying 
        the measurment by this number will therefore give us millig (read, 
        "milli g's").  Divide this result by 1000 to get g's.
    */
    putDataHere->X = (float)localX * (3.9 / 1000.0);//(4.0 / 1024.0);
    putDataHere->Y = (float)localY * (3.9 / 1000.0);//(4.0 / 1024.0);
    putDataHere->Z = (float)localZ * (3.9 / 1000.0);//(4.0 / 1024.0);

    mLogInstance.LogStrFloat("accel::readAllAxes - accel X = ", putDataHere->X);
    mLogInstance.LogStrFloat("accel::readAllAxes - accel Y = ", putDataHere->Y);
    mLogInstance.LogStrFloat("accel::readAllAxes - accel Z = ", putDataHere->Z);
}

#endif  // ACCEL_MODEL==ADXL345
#endif  // HAVE_ACCEL
