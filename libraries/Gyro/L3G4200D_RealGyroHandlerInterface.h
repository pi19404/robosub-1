#ifndef __L3G4200D_REAL_GYRO_HANDLER_INTERFACE_H__
#define __L3G4200D_REAL_GYRO_HANDLER_INTERFACE_H__

#include "GyroHandler.h"
#include "L3G4200D_GyroCfgData.h"

template <class T>
class RealGyroHandlerInterface 
    :
    public GyroHandler<T>
{
    typedef T data_type;
    
    public:
        using GyroHandler<data_type>::AttachOutputLine;
        
        // From DeviceHandler
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
        virtual void SetCtrlReg1( const GyroCtrlReg1& ) = 0;
        virtual void SetCtrlReg2( const GyroCtrlReg2& ) = 0;
        virtual void SetCtrlReg3( const GyroCtrlReg3& ) = 0;
        virtual void SetCtrlReg4( const GyroCtrlReg4& ) = 0;
        virtual void SetCtrlReg5( const GyroCtrlReg5& ) = 0;
        
        // StatusReg
        virtual const GyroStatusReg& GetStatus() = 0;
        
        // FifoCtrlReg
        virtual void SetFifoCtrlReg( const GyroFifoCtrlReg& ) = 0;
        
        // FifoSrcReg
        virtual const GyroFifoSrcReg& GetFifoStatus() = 0;
        
        // Int1CfgReg
        virtual void SetInt1Config( const GyroInt1CfgReg& ) = 0;
        
        // Int1SrcReg
        virtual const GyroInt1SrcReg& GetInt1Status() = 0;
        
        // Thresholds
        virtual void SetThresholdX( uint16_t ) = 0;
        virtual void SetThresholdY( uint16_t ) = 0;
        virtual void SetThresholdZ( uint16_t ) = 0;
        
        // Int1Duration
        virtual void SetInt1Duration( const GyroInt1Duration& ) = 0;
        
    protected:
        using GyroHandler<data_type>::mOPipes;
};

#endif //__L3G4200D_REAL_GYRO_HANDLER_INTERFACE_H__
