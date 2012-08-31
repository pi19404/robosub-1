#ifndef __ANALOG_DEVICE_H__
#define __ANALOG_DEVICE_H__

#include "DeviceHandler.h"

class AnalogDevice
    :
    public DeviceHandler
{
    public:
        AnalogDevice( int analogPin )
            :
            mAnalogPin(analogPin)
        { }
        
        virtual void Initialize()
        {
            pinMode(mAnalogPin, INPUT);
        }
        
        virtual int Read()
        {
            return analogRead(mAnalogPin);
        }
    private:
        int mAnalogPin;
};

#endif //__ANALOG_DEVICE_H__