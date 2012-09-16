#ifndef __COMPASS_HANDLER_H__
#define __COMPASS_HANDLER_H__

#include "CompassData.h"
#include "../Device/MeasurementDevice.h"
#include "../Pipeline/OPipedDevice.h"

template <class T>
class CompassHandler
    :
     public MeasurementDevice
    ,public OPipedDevice< CompassData<T> >
{
    typedef CompassData<T> data_type;
    
    public:
        using OPipedDevice<data_type>::AttachOutputLine;
        
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
    protected:  
        using OPipedDevice<data_type>::mOPipes;
};

#endif //__COMPASS_HANDLER_H__
