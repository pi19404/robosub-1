#ifndef __REAL_DEPTH_SENSOR_H__
#define __REAL_DEPTH_SENSOR_H__

#include "DepthSensor.h"

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include <stddef.h>
#include <stdint.h>

#define DEPTH_SENSOR_CONV_FACTOR 1

class RealDepthSensor
    :
    public DepthSensor<float>
{
    typedef float data_type;
    
    public:
        RealDepthSensor( int readPin )
            :
             DepthSensor<data_type>()
            ,mReadPin(readPin)
            ,mDepthData(0)
        {
      
        }
        
        virtual void Initialize()
        {
            pinMode( mReadPin, INPUT );
            
            // do first read
            GetData();
        }
        
        virtual void GetData()
        {
            mDepthData = analogRead(mReadPin);
            data_type depth = mDepthData * DEPTH_SENSOR_CONV_FACTOR;
                        
            for( size_t i = 0; i < mOPipes.Size(); ++i )
            {
                mOPipes[i].WriteTo( depth );
            }
        }
        
    protected:
        using DepthSensor<data_type>::mOPipes;
        
    private:
        int     mReadPin;
        int16_t mDepthData;
};

#endif //__DEPTH_SENSOR_H__
