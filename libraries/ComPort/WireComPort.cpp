#include "WireComPort.h"
#include "../Logging/LogManager.h"

// the else is for a backwards compatability check; see here
// http://arduino.cc/en/Main/ReleaseNotes
//#if defined (ARDUINO) &&
    #if (ARDUINO >= 100)
        #define __WIRE_WRITE(x) mWire.write(x)
        #define __WIRE_READ(x)  mWire.read(x)
        #include <Arduino.h>
//    #endif
#else
    #define __WIRE_WRITE(x) mWire.send(x)
    #define __WIRE_READ(x)  mWire.receive(x)
    #include <WProgram.h>
#endif

WireComPort::WireComPort( TwoWire& wire, uint8_t device, unsigned long timeOutMillis )
    :
ComPort(device)
    ,mWire(wire)
    ,mTimeOut(timeOutMillis)
{
    // Nothing left to do here.
}

WireComPort::~WireComPort()
{
    // Nothing left to do here.
}

void WireComPort::write(uint8_t *data, uint8_t nBytes)
{
    if( nBytes )
    {
        mWire.beginTransmission(mDevice);
        for( int i = 0; i < nBytes; ++i )
        {
            __WIRE_WRITE(data[i]);
        }
        mWire.endTransmission();
    }
}

void WireComPort::read(uint8_t *data, uint8_t nBytes)
{    
    read(0, 0, data, nBytes);
}

void WireComPort::read(uint8_t* wrdata, uint8_t w_nBytes, uint8_t* rddata, uint8_t r_nBytes)
{
    // Do write part
    if( w_nBytes )
    {
        mWire.beginTransmission(mDevice);
        for( int i = 0; i < w_nBytes; ++i )
        {
            __WIRE_WRITE( wrdata[i] );
        }
        mWire.endTransmission();
    }

    // Do read part
    mWire.requestFrom(mDevice, r_nBytes);

    // Timeout if Port Not Available
    unsigned long start = millis();
    while(!mWire.available())
    { 
        if( (millis() - start) >= mTimeOut )
        {
            LogManager& _lm = LogManager::GetInstance();
            _lm.LogStr("-I2C Read TimeOut for Device: 0x" + String(mDevice, HEX) );
            break;
        }
    }
    int i = 0;
    while(mWire.available())
    {
        if( i < r_nBytes)
        {
            rddata[i] = __WIRE_READ();
        }
        else
        {
            __WIRE_READ();
        }
        ++i;
    }
}
