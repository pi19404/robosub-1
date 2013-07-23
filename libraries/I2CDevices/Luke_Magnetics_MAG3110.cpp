#ifdef HAVE_MAGNETICS
#ifdef COMPASS_IS_MAG3110

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// for declaration of the accelerometer class
#include "Magnetics.h"

// for use of the Arduino "Wire" object
#include <Wire.h>

// these are definitions for the heading read operation described on pages 13 
// and 14 of the HMC6343 datasheet
#define MAG_BASE_WRITE      0x1C
#define MAG_BASE_READ       0x1D
#define MAG_CONFIG_1        0x10
#define MAG_CONFIG_2        0x11
#define CMD_READ_XYZ        0x01
#define CMD_READ_DELAY      3
#define CMD_READ_XYZ_BYTES  6

#define I2C_SLAVE_SELECTORW 0xE0
#define I2C_SLAVE_SELECTORR 0xE1

// according to the example communication on page 13 of the chip manual, it 
// defaults to Run Mode, so no initialization is needed (a constructor is still
// required, of course)
magnetics::magnetics()
{
}

void selectSensor(uint8_t sensorId)
{
    uint8_t configByte = 1 << sensorId;
	
    Wire.beginTransmission(I2C_SLAVE_SELECTORW);
    Wire.send(configByte);
    Wire.endTransmission();
    
    // Validate
    /*
    Wire.requestFrom(I2C_SLAVE_SELECTORR, 1);
    configByte = Wire.recieve();
    
    mLogInstance.LogStrHex("compass::selectSensor - ConfigByte = ", configByte);
    */
}

void magnetics::initMagnetics()
{
    Wire.beginTransmission(MAG_BASE_WRITE); // transmit to device 0x0E
    Wire.send(MAG_CONFIG_1);              // cntrl register2
    Wire.send(0x80);              // send 0x80, enable auto resets
    Wire.endTransmission();       // stop transmitting
    
    delay(15);
    
    Wire.beginTransmission(MAG_BASE_WRITE); // transmit to device 0x0E
    Wire.send(MAG_CONFIG_2);              // cntrl register1
    Wire.send(0x59); 
    Wire.endTransmission();       // stop transmitting
}

void magnetics::readAllAxes(COMPASS_DATA *putDataHere)
{
    // read and return all accelerometer handler data
    int i;
    uint8_t dataBuffer[CMD_READ_RCV_BYTES];
    uint16_t localX;
    uint16_t localY;
    uint16_t localZ;
    
    int16_t *localXs = &localX;
    int16_t *localYs = &localY;
    int16_t *localZs = &localZ;

    // start up an instance of the log manager 
    LogManager& mLogInstance = LogManager::GetInstance();

    if (NULL == putDataHere)
    {
        mLogInstance.LogStr("magnetics::readAllAxes - null argument; aborting");
        putDataHere->x = 0.0;
        putDataHere->y = 0.0;
        putDataHere->z = 0.0;
        return;
    }

    // initialize local variables
    for (i = 0; i < CMD_READ_XYZ_BYTES; i++)
    {
        dataBuffer[i] = 0;
    }
    localX = 0;
    localY = 0;
    localZ = 0;

    // send the command and read the data
    Wire.beginTransmission(MAG_BASE_WRITE);
    Wire.send(CMD_READ_XYZ);
    Wire.endTransmission();
    
    delayMicroseconds(CMD_READ_DELAY);
    // Read all 6 bytes
    Wire.requestFrom(MAG_BASE_READ, (uint8_t)CMD_READ_XYZ_BYTES);
    for (i = 0; i < CMD_READ_XYZ_BYTES; i++)
    {
        dataBuffer[i] = Wire.receive();
    }
    Wire.endTransmission();

    // now assemble the data
    localX = dataBuffer[0] << 8;
    localX |= dataBuffer[1];
    localY = dataBuffer[2] << 8;
    localY |= dataBuffer[3];
    localZ = dataBuffer[4] << 8;
    localZ |= dataBuffer[5];

    // now apply the binary -> float conversion factor and put the data into 
    // the provided data structure;

    putDataHere->x = (float)(*localXs) * (1.0);
    putDataHere->y = (float)(*localYs) * (1.0);
    putDataHere->z = (float)(*localZs) * (1.0);

    mLogInstance.LogStrFloat("compass::readAllAxes - X = ", putDataHere->x);
    mLogInstance.LogStrFloat("compass::readAllAxes - Y = ", putDataHere->y);
    mLogInstance.LogStrFloat("compass::readAllAxes - Z = ", putDataHere->z);
}

#endif  // COMPASS_IS_HMC6343
#endif  // HAVE_COMPASS
