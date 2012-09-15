#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "DeviceHandler.h"
#include "../Containers/Array.h"

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

class Interrupt
    :
    public DeviceHandler
{
    public:
        Interrupt( int intNumber, int intPin )
            :
             mIntNum(intNumber)
            ,mIntPin(intPin)
        {
        
        }
        
        virtual void SetHandler( void (&handler)() )
        {
            attachInterrupt( mIntNum, &handler, CHANGE );
        }
        
        virtual void Initialize()
        {
          pinMode(mIntPin, INPUT);
        }
          
    private:
        volatile int mIntNum;
        volatile int mIntPin;
};

#endif //__INTERRUPT_H__
