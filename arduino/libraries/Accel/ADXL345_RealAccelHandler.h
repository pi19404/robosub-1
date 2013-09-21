#ifndef __ADXL345_REAL_ACCEL_HANDLER__
#define __ADXL345_REAL_ACCEL_HANDLER__

#ifndef __REAL_ACCEL_HANDLER_H__
    #error Must be included from RealAccelHandler.h
#endif

#include "../ComPort/ComPort.h"
#include "ADXL345_RealAccelHandlerInterface.h"

class RealAccelHandler 
    :
    public RealAccelHandlerInterface<float>
{
    typedef float data_type;
    
    public:
        using RealAccelHandlerInterface<data_type>::AttachOutputLine;
        
        RealAccelHandler( ComPort& );
        
        virtual void Initialize();
        virtual void GetData();
        
        virtual void SetXOffsetInMillig( data_type );
        virtual void SetYOffsetInMillig( data_type );
        virtual void SetZOffsetInMillig( data_type );
        virtual void SetTapDurationInMilliseconds( data_type );
        virtual void SetLatencyInMiliseconds( data_type );       
        virtual void SetTapWindowInMilliseconds( data_type );
        virtual void SetTapThresholdInMillig( data_type );
        virtual void SetActivityThresholdInMillig( data_type );
        virtual void SetInactivityThresholdInMillig( data_type );
        virtual void SetInactivityTimeInSeconds( data_type );
        virtual void SetActInactCtrl( const ActInactCtrlReg& );
        virtual void SetFreeFallThresholdInMillig( data_type );
        virtual void SetFreeFallTimeInMilliseconds( data_type );
        virtual void SetTapAxesControl( const TapAxesCtrlReg& ) ;
        virtual const ActTapSrcReg& GetActTapSrc();
        virtual void SetDataRatePwrModeCtrl( const DataRatePwrModeCtrlReg& );
        virtual void SetPwrCtrl( const PwrCtrlReg& );
        virtual void SetIntCtrl( const IntCtrlReg& );
        virtual void SetIntMap( const IntMapReg& );
        virtual const IntSrcReg& GetIntSrc();
        virtual void SetDataFormat( const DataFormatReg& );
        virtual void SetFifoCtrl( const FifoCtrlReg& );
        virtual const FifoStatusReg& GetFifoStatus();
        
    private:
        void    WriteReg( RegNames::Enum, uint8_t );
        uint8_t ReadReg( RegNames::Enum );
        void    DoGetData();
        
    private:
        using RealAccelHandlerInterface< data_type >::mOPipes;
        
        ComPort&            mComPort;
        AccelData<int16_t>  mAccelData;
};

#include "ADXL345_RealAccelHandler.cpp"

#endif //__ADXL345_REAL_ACCEL_HANDLER__
