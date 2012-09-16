#ifndef __L3G4200D_REAL_GYRO_HANDLER_H__
#define __L3G4200D_REAL_GYRO_HANDLER_H__

#ifndef __REAL_GYRO_HANDLER_H__
    #error Must be included from RealGyroHandler.h
#endif

#include "L3G4200D_RealGyroHandlerInterface.h"

class RealGyroHandler
    :
    public RealGyroHandlerInterface<float>
{
    typedef float data_type;
    
    public:
        RealGyroHandler( ComPort& comPort );
        
        using RealGyroHandlerInterface<data_type>::AttachOutputLine;
        
        // From DeviceHandler
        virtual void Initialize();
        virtual void GetData();
        
        virtual void SetCtrlReg1( const GyroCtrlReg1& );
        virtual void SetCtrlReg2( const GyroCtrlReg2& );
        virtual void SetCtrlReg3( const GyroCtrlReg3& );
        virtual void SetCtrlReg4( const GyroCtrlReg4& );
        virtual void SetCtrlReg5( const GyroCtrlReg5& );
        
        // StatusReg
        virtual const GyroStatusReg& GetStatus();
        
        // FifoCtrlReg
        virtual void SetFifoCtrlReg( const GyroFifoCtrlReg& );
        
        // FifoSrcReg
        virtual const GyroFifoSrcReg& GetFifoStatus();
        
        // Int1CfgReg
        virtual void SetInt1Config( const GyroInt1CfgReg& );
        
        // Int1SrcReg
        virtual const GyroInt1SrcReg& GetInt1Status();
        
        // Thresholds
        virtual void SetThresholdX( uint16_t );
        virtual void SetThresholdY( uint16_t );
        virtual void SetThresholdZ( uint16_t );
        
        // Int1Duration
        virtual void SetInt1Duration( const GyroInt1Duration& );
            
    private:
        void WriteReg( GyroRegNames::Enum, uint8_t );
        uint8_t ReadReg( GyroRegNames::Enum );
        void DoGetData();
        
    private:
        using RealGyroHandlerInterface<data_type>::mOPipes;
        
        GyroData<int16_t>   mGyroData;
        ComPort&            mComPort;
        
};

#endif //__L3G4200D_REAL_GYRO_HANDLER_H__
