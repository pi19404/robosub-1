#ifdef HAVE_COMPASS
#ifdef COMPASS_IS_HMC6343

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// for declaration of the accelerometer class
#include "Compass.h"

// for use of the Arduino "Wire" object
#include <Wire.h>

// these are definitions for the heading read operation described on pages 13 
// and 14 of the HMC6343 datasheet
#define CPS_BASE_WRITE      0x32
#define CPS_BASE_READ       0x33
#define CMD_READ            0x50
#define CMD_READ_DELAY      500
#define CMD_READ_RCV_BYTES  6

// according to the example communication on page 13 of the chip manual, it 
// defaults to Run Mode, so no initialization is needed (a constructor is still
// required, of course)
compass::compass()
{
}

void compass::readAllAxes(COMPASS_DATA *putDataHere)
{
    // read and return all accelerometer handler data
    int i;
    uint8_t dataBuffer[CMD_READ_RCV_BYTES];
    uint16_t localHeading;
    uint16_t localPitch;
    uint16_t localRoll;

    // start up an instance of the log manager 
    LogManager& mLogInstance = LogManager::GetInstance();

    if (NULL == putDataHere)
    {
        mLogInstance.LogStr("compass::readAllAxes - null argument; aborting");
        putDataHere->heading = 0.0;
        putDataHere->pitch = 0.0;
        putDataHere->roll = 0.0;
        return;
    }

    // initialize local variables
    for (i = 0; i < CMD_READ_RCV_BYTES; i++)
    {
        dataBuffer[i] = 0;
    }
    localHeading = 0;
    localPitch = 0;
    localRoll = 0;

    // send the command and read the data
    Wire.beginTransmission(CPS_BASE_WRITE);
    Wire.send(CMD_READ);
    delay(CMD_READ_DELAY);
    //Wire.send(0x33);
    Wire.requestFrom(CPS_BASE_READ, (uint8_t)CMD_READ_RCV_BYTES);
    for (i = 0; i < CMD_READ_RCV_BYTES; i++)
    {
        dataBuffer[i] = Wire.receive();
    }
    Wire.endTransmission();

    // now assemble the data
    localHeading = dataBuffer[0] << 8;
    localHeading |= dataBuffer[1];
    localPitch = dataBuffer[2] << 8;
    localPitch |= dataBuffer[3];
    localRoll = dataBuffer[4];
    localRoll |= dataBuffer[5];

    // now apply the binary -> float conversion factor and put the data into 
    // the provided data structure;
    // the conversion factor is calculated by dividing the total measurement 
    // range and dividing it by 2^(total number of bits used to specify that 
    // range);
    // each piece of data is assumed to be 16 bits (not specified in the 
    // manual), and the HMC6343 example on page 13 of the manual says that the
    // range of the heading is 0 - 3600 (so should not expect a negative 
    // value), and the range of the pitch and roll are both +-900 (so should
    // expect a negative value)

    putDataHere->heading = (float)localHeading * (3600.0 / 65536.0);
    putDataHere->pitch = (float)localPitch * (1800.0 / 65536.0);
    putDataHere->roll = (float)localRoll * (1800.0 / 65536.0);

    mLogInstance.LogStrFloat("compass::readAllAxes - heading = ", putDataHere->heading);
    mLogInstance.LogStrFloat("compass::readAllAxes - pitch = ", putDataHere->pitch);
    mLogInstance.LogStrFloat("compass::readAllAxes - roll = ", putDataHere->roll);
}

#endif  // COMPASS_IS_HMC6343
#endif  // HAVE_COMPASS
