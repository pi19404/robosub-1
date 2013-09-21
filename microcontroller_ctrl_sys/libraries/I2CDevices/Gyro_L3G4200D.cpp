#ifdef HAVE_GYRO
#if GYRO_MODEL==L3G4200D

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// for declaration of the gyroscope class
#include "Gyro.h"

// for use of the Arduino "Wire" object
#include <Wire.h>

// these are the addresses for the registers that we will be using
// Note: axis registers are appended with 'L' and 'H' because that is the way
// that they appear in the register tables in the microchip's manual
#define GYRO_BASE       0x69    // apparently, SDO is connected to VCC
#define GYRO_CTRL_REG1  0x20
#define GYRO_XL         0x28
#define GYRO_XH         0x29
#define GYRO_YL         0x2A
#define GYRO_YH         0x2B
#define GYRO_ZL         0x2C
#define GYRO_ZH         0x2D

/*
The device should only need to be initialized once, and that is on powerup.  We 
are not handling the case where something gets unplugged and has to be 
reconnected.  If that happens, just reconnect the device and power cycle the 
microcontroller, and then things will reinitialize.
*/
gyro::gyro()
{
    uint8_t dataByte;

    LogManager& mLogInstance = LogManager::GetInstance();

    // read control register 1, set the mode bit from standby to measure, then 
    // write back the modified register

    mLogInstance.LogStr("gyro::gyro - begin reading ctrl reg 1");
    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_CTRL_REG1, &dataByte);
    mLogInstance.LogStrHex("gyro::gyro - done reading ctrl reg 1: ", (int)dataByte);
    dataByte |= 0x08;
    mLogInstance.LogStrHex("gyro::gyro - begin writing back ctrl reg 1: ", (int)dataByte);
    writeDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_CTRL_REG1, dataByte);
    mLogInstance.LogStr("gyro::gyro - done writing back ctrl reg 1");
}

void gyro::readDeviceRegister
    (uint8_t devAddr, uint8_t regAddr, uint8_t *dataByte)
{
    // start up a local instance of the log manager
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("gyro::readDeviceRegister - begin");
    Wire.beginTransmission(devAddr);
    Wire.send(regAddr);
    Wire.endTransmission();

    mLogInstance.LogStr("gyro::readDeviceRegister - requesting data");
    Wire.requestFrom(devAddr, (uint8_t)1);

    mLogInstance.LogStr("gyro::readDeviceRegister - wait for data");
    Wire.beginTransmission(devAddr);
    while(!Wire.available());
    while(Wire.available())
    {
        *dataByte = Wire.receive();
    }
    Wire.endTransmission();
    mLogInstance.LogStrHex("gyro::readDeviceRegister - got data: ", (int)dataByte);
}

void gyro::writeDeviceRegister
    (uint8_t devAddr, uint8_t regAddr, uint8_t dataByte)
{
    // start up a local instance of the log manager
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStrHex("gyro::writeDeviceRegister - begin writing ", (int)dataByte);
    Wire.beginTransmission(devAddr);
    Wire.send(regAddr);
    Wire.send(dataByte);
    Wire.endTransmission();
    mLogInstance.LogStr("accel::writeDeviceRegister - done");
}

void gyro::readAllAxes(GYRO_DATA *putDataHere)
{
    // read and return all gyroscope handler data
    uint8_t dataByte;
    int16_t localX; 
    int16_t localY;
    int16_t localZ;

    // start up an instance of the log manager 
    LogManager& mLogInstance = LogManager::GetInstance();

    if (NULL == putDataHere)
    {
        mLogInstance.LogStr("gyro::readAllAxes - null argument; aborting");
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
    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_XH, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read XH = ", dataByte);
    localX = dataByte;
    localX = localX << 8;

    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_XL, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read XL = ", dataByte);
    localX |= dataByte;
//    mLogInstance.LogStrHex("gyro::readAllAxes - localX = ", localX);

    // now read the Y registers.
    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_YH, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read YH = ", dataByte);
    localY = dataByte << 8;

    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_YL, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read YL = ", dataByte);
    localY |= dataByte;
//    mLogInstance.LogStrHex("gyro::readAllAxes - localY = ", localY);

    // now read the Z registers.
    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_ZH, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read ZH = ", dataByte);
    localZ = dataByte << 8;

    readDeviceRegister((uint8_t)GYRO_BASE, (uint8_t)GYRO_ZL, &dataByte);
//    mLogInstance.LogStrHex("gyro::readAllAxes - Read ZL = ", dataByte);
    localZ |= dataByte;
//    mLogInstance.LogStrHex("gyro::readAllAxes - localZ = ", localZ);


    // now apply the binary -> float conversion factor and put the data into 
    // the provided data structure
    /*
        We want our units are in Degrees Per Second (dps).

        On page 10 of the gyro chip's datasheet, the sensitivity for a 
        measurement range of 250dps, which is the default and what we are 
        using, is 8.75.  Multiplying the measurment by this number will give us
        millidps.  Divide this result by 1000 to get dps.
    */
    putDataHere->X = (float)localX * (8.75 / 1000.0);
    putDataHere->Y = (float)localY * (8.75 / 1000.0);
    putDataHere->Z = (float)localZ * (8.75 / 1000.0);

    mLogInstance.LogStrFloat("gyro::readAllAxes - gyro X = ", putDataHere->X);
    mLogInstance.LogStrFloat("gyro::readAllAxes - gyro Y = ", putDataHere->Y);
    mLogInstance.LogStrFloat("gyro::readAllAxes - gyro Z = ", putDataHere->Z);
}

#endif  // GYRO_MODEL==L3G4200D
#endif  // HAVE_GYRO
