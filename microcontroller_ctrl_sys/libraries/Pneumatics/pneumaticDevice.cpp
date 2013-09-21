#include <Pneumatics/pneumaticDevice.h>
#include <Logging/LogManager.h>
#include <WProgram.h>

pDev::pDev()
{
    // initialize to an invalid number
    mPinNum = -1;
}

void pDev::init(int pinNum)
{
    mPinNum = pinNum;
    pinMode(mPinNum, OUTPUT);

    // set default mode of low
    digitalWrite(mPinNum, LOW);
}

void pDev::activate(void)
{
    digitalWrite(mPinNum, HIGH);
}

void pDev::deactivate(void)
{
    digitalWrite(mPinNum, LOW);
}


