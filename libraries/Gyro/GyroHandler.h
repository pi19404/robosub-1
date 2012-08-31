#ifndef __GYRO_HANDLER_H__
#define __GYRO_HANDLER_H__

#include "GyroData.h"
#include "..\Device\MeasurementDevice.h"
#include "..\Pipeline\OPipedDevice.h"

template <class T>
class GyroHandler
    :
    public MeasurementDevice
   ,public OPipedDevice< GyroData<T> >
    
{
    typedef GyroData<T> data_type;
    
    public:
        using OPipedDevice<data_type>::AttachOutputLine;
        
        // From Measurement Device
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
    protected:
    
        using OPipedDevice<data_type>::mOPipes;
};

#endif //__GYRO_HANDLER_H__