#ifndef __HMC6343_REAL_COMPASS_HANDLER_H__
#define __HMC6343_REAL_COMPASS_HANDLER_H__

// Only include this file from RealCompassHandler.h
#ifndef __REAL_COMPASS_HANDLER_H__
    #error Should only be included from RealCompassHandler.h
#endif

#include "HMC6343_RealCompassHandlerInterface.h"

class RealCompassHandler
      :
       public RealCompassHandlerInterface<float>
{    
    typedef float data_type;
    
    public:
        RealCompassHandler( ComPort& );
        ~RealCompassHandler();
        
        using RealCompassHandlerInterface<data_type>::AttachOutputLine;
        
        // From HMC6343 CompassHandler
        virtual void Initialize();
        virtual void GetData();
        
        virtual void SetOrientationLevel();
        virtual void SetOrientationUprightEdge();
        virtual void SetOrientationUprightFront();
        
        virtual void EnterRunMode();
        virtual void EnterStandbyMode();
        virtual void EnterCalibrationMode();
        virtual void ExitCalibrationMode();
        virtual void ResetDevice();
        virtual void EnterSleepMode();
        virtual void ExitSleepMode();
        
        // EEPROM-commands (Persistent)
        virtual void              SetConfig( CompassConfig& );
        virtual CompassConfig     GetConfig();
        virtual CompassRate::Enum GetMeasurementRate();
        virtual void              SetMeasurementRate( CompassRate::Enum );   

        
    private:
        void        EepromWrite( uint8_t, uint8_t );
        uint8_t     EepromRead( uint8_t  );      
        void        DoSendCommand( uint8_t );
        void        SetDeviceConfig();
        void        GetDeviceConfig();
        void        SetDeviceRate();
        void        GetDeviceRate();
        
        void        DoGetData();
        
    private:
        using RealCompassHandlerInterface<data_type>::mOPipes;

        CompassData<int16_t> mCompassData;
        CompassConfig        mDeviceConfig;
        CompassRate          mDeviceRate;
        ComPort&             mComPort;
};

#endif //__HMC6343_REAL_COMPASS_HANDLER_H__
