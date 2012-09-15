#ifndef __MEASUREMENT_DEVICE_H__
#define __MEASUREMENT_DEVICE_H__

#include "../Device/DeviceHandler.h"

class MeasurementDevice
    :
    public DeviceHandler
{
    public:    
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
};

#endif //__MEASUREMENT_DEVICE_H__
