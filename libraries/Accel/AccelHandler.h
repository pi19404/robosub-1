#ifndef __ACCEL_HANDLER_H__
#define __ACCEL_HANDLER_H__

#include "AccelData.h"
#include "../Device/MeasurementDevice.h"
#include "../Pipeline/OPipedDevice.h"

template <class T>
class AccelHandler
    :
     public MeasurementDevice
    ,public OPipedDevice< AccelData<T> >
{
    typedef AccelData<T> data_type;
    
    public:
        using OPipedDevice<data_type>::AttachOutputLine;
        
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
    protected:
        using OPipedDevice<data_type>::mOPipes;
};

#endif //__ACCEL_HANDLER_H__
