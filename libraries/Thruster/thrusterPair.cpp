#include <Logging/LogManager.h>

#include <WProgram.h>

#include "thrusterPair.h"
#include "string.h"

thrusterPair::thrusterPair()
{
    // initialize to invalid values
    mAddr = -1;
}

void thrusterPair::init(unsigned char hBridgeAddr)
{
    // if an invalid address is sent out the two wire signal line, the motors
    // will just ignore it, so the worst that will happen is that the motors 
    // won't respond (that is, no system crash);
    // therefore, it was determined that this value did not need to be checked
    mAddr = hBridgeAddr;

    // initialize both motor outputs to no duty cycle
    // command 0 means drive motor 1 forward (I will call this our default)
    Serial1.write((uint8_t)mAddr);                      // h bridge address
    Serial1.write((uint8_t)0);                          // command
    Serial1.write((uint8_t)0);                          // command data
    Serial1.write((uint8_t)((mAddr + 0 + 0) & 127));    // checksum

    // command 4 means drive motor 1 forward (I will call this our default)
    Serial1.write((uint8_t)mAddr);                      // h bridge address
    Serial1.write((uint8_t)4);                          // command
    Serial1.write((uint8_t)0);                          // command data
    Serial1.write((uint8_t)((mAddr + 4 + 0) & 127));    // checksum
}

void thrusterPair::setMotor(int motorNum, int dutyCycle, int direction)
{
    // Note: a command of '0' drives motor 1 forward, but if the command data
    // is also 0, then we won't have a problem
    unsigned char command = 0;
    unsigned char commandData = 0;
    unsigned char checkSum = 0;

    LogManager& mLogInstance = LogManager::GetInstance();

    // check the duty cycle argument and force it to be within range
    /*
        We decided that we did not want the h bridges to run at 100% duty cycle
        because, even in water, the thrusters  will burn up in a couple minutes 
        due to the strain of that much current going through them.  So 
        constrain the h bridges to a maximum of 85% duty cycle in the final 
        release.
    */
    if (dutyCycle < 0)
    {
        dutyCycle = 0;
    } 
    else if (dutyCycle > 80)
    {
        dutyCycle = 80;
    }

    // set command based on motor number and direction
    // possible commands:
    // 0 - drive motor 1 forward
    // 1 - drive motor 1 backward
    // 4 - drive motor 2 forward
    // 5 - drive motor 2 backward
    if (1 == motorNum)
    {
        if (direction < 0)
        {
            // motor 1 backwards
            command = 1;
        }
        else 
        {
            // motor 1 forward
            command = 0;
        }
    }
    else if (2 == motorNum)
    {
        if (direction < 0)
        {
            // motor 2 backwards
            command = 5;
        }
        else 
        {
            // motor 1 forward
            command = 4;
        }
    }
    else
    {
        // motor number is bad; ignore command and abort
        mLogInstance.LogStr("thrusterPair::setMotor - bad motor number.");
        return;
    }

    // calculate the command data from the duty cycle
    // allowable data values are between 0 and 127, so the conversion from duty
    // to command data is 1.27 command data units per duty percentage;
    commandData = dutyCycle * 1.27;

    // assemble a checksum according to the example given in the sabertooth2x12
    // manual, page 19
    checkSum = (mAddr + command + commandData) & 127;

    // send the data out the serial port
    // Note: this serial port MUST be initialized first (likely done in Arduino
    // sketch in the setup)
    Serial1.write(mAddr);
    Serial1.write(command);
    Serial1.write(commandData);
    Serial1.write(checkSum);
}

void thrusterPair::stopMotor(int motorNum)
{
    // Note: to stop a motor, you only need to set the command data to 0, but 
    // I would like to "reset" the direction to forwards anyway
    unsigned char command = 0;
    unsigned char commandData = 0;
    unsigned char checkSum = 0;

    // set duty cycle of specified motor to 0
    if (1 == motorNum)
    {
        command = 0;
    }
    else if (2 == motorNum)
    {
        command = 4;
    }
    else
    {
        // bad motor number; ignore command and abort
        return;
    }

    // assemble a checksum according to the example given in the sabertooth2x12
    // manual, page 19
    checkSum = (mAddr + command + commandData) & 127;

    Serial1.write(mAddr);
    Serial1.write(command);
    Serial1.write(commandData);
    Serial1.write(checkSum);
}







