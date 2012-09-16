#ifndef __HMC6343_REAL_COMPASS_HANDLER_INTERFACE_H__
#define __HMC6343_REAL_COMPASS_HANDLER_INTERFACE_H__

// Only include this file from RealCompassHandler.h
#ifndef __REAL_COMPASS_HANDLER_H__
    #error Should only be included from RealCompassHandler.h
#endif

#include "CompassHandler.h"
#include "HMC6343_CompassCfgData.h"

////////////////////////////////////////////////////////
// class Compass Handler
// Device Handler for Honeywell HMC6343Tilt-Compensated 
// Compass is a:
//             - Compass                
//             - 3-axis magnetometer    
//             - 3-axis accelerometer.  
//             - 3-axis tilt sensor.    
//             - temperature sensor.    
////////////////////////////////////////////////////////
template <class T>
class RealCompassHandlerInterface
    :
    public CompassHandler<T>
{
    
    public:
        using CompassHandler<T>::AttachOutputLine;
    
        // From MeasurementDevice:
        virtual void Initialize()                         = 0;
        virtual void GetData()                            = 0;
        
        // For HMC6343's to Implement:
        virtual void SetOrientationLevel()                = 0;
        virtual void SetOrientationUprightEdge()          = 0;
        virtual void SetOrientationUprightFront()         = 0;
        
        virtual void EnterRunMode()                       = 0;
        virtual void EnterStandbyMode()                   = 0;
        virtual void EnterCalibrationMode()               = 0;
        virtual void ExitCalibrationMode()                = 0;
        virtual void ResetDevice()                        = 0;
        virtual void EnterSleepMode()                     = 0;
        virtual void ExitSleepMode()                      = 0;
        
        // EEPROM-commands (Persistent)
        virtual void              SetConfig( CompassConfig& )             = 0;
        virtual CompassConfig     GetConfig()                             = 0;
        virtual CompassRate::Enum GetMeasurementRate()                    = 0;
        virtual void              SetMeasurementRate( CompassRate::Enum ) = 0;     

    protected:
        using CompassHandler<T>::mOPipes;
};


// Include Implementation
#include "HMC6343_RealCompassHandler_impl.h"

#endif //__HMC6343_REAL_COMPASS_HANDLER_INTERFACE_H__
