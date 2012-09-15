#ifndef __DEPTH_SENSOR_H__
#define __DEPTH_SENSOR_H__

#include "../Device/MeasurementDevice.h"
#include "../Pipeline/OPipedDevice.h"

template <class T>
class DepthSensor
    :
     public MeasurementDevice
    ,public OPipedDevice<T>
{
    typedef T data_type;
    public:
        using OPipedDevice<data_type>::AttachOutputLine;
        
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
    protected:
        using OPipedDevice<data_type>::mOPipes;
};

#endif //__DEPTH_SENSOR_H__
