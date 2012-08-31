#ifndef __ADXL345_REAL_ACCEL_HANDLER_INTERFACE_H__
#define __ADXL345_REAL_ACCEL_HANDLER_INTERFACE_H__

#ifndef __REAL_ACCEL_HANDLER_H__
    #error Must be included from RealAccelHandler.h
#endif

#include "AccelHandler.h"
#include "ADXL345_AccelCfgData.h"

using namespace Accel;

template <class T>
class RealAccelHandlerInterface
    :
    public AccelHandler<T>
{
    typedef T data_type;
 
    public:
        using AccelHandler<data_type>::AttachOutputLine;
        
        virtual void Initialize() = 0;
        virtual void GetData() = 0;
        
        virtual void SetXOffsetInMillig( data_type ) = 0;
        virtual void SetYOffsetInMillig( data_type ) = 0;
        virtual void SetZOffsetInMillig( data_type ) = 0;
        virtual void SetTapDurationInMilliseconds( data_type ) = 0;
        virtual void SetLatencyInMiliseconds( data_type ) = 0;       
        virtual void SetTapWindowInMilliseconds( data_type ) = 0;
        virtual void SetTapThresholdInMillig( data_type ) = 0;
        virtual void SetActivityThresholdInMillig( data_type ) = 0;
        virtual void SetInactivityThresholdInMillig( data_type ) = 0;
        virtual void SetInactivityTimeInSeconds( data_type ) = 0;
        virtual void SetActInactCtrl( const ActInactCtrlReg& ) = 0;
        virtual void SetFreeFallThresholdInMillig( data_type ) = 0;
        virtual void SetFreeFallTimeInMilliseconds( data_type ) = 0;
        virtual void SetTapAxesControl( const TapAxesCtrlReg& )  = 0;
        virtual const ActTapSrcReg& GetActTapSrc() = 0;
        virtual void SetDataRatePwrModeCtrl( const DataRatePwrModeCtrlReg& ) = 0;
        virtual void SetPwrCtrl( const PwrCtrlReg& ) = 0;
        virtual void SetIntCtrl( const IntCtrlReg& ) = 0;
        virtual void SetIntMap( const IntMapReg& ) = 0;
        virtual const IntSrcReg& GetIntSrc() = 0;
        virtual void SetDataFormat( const DataFormatReg& ) = 0;
        virtual void SetFifoCtrl( const FifoCtrlReg& ) = 0;
        virtual const FifoStatusReg& GetFifoStatus() = 0;
           
    protected:
        using AccelHandler<data_type>::mOPipes;
};

// Include implementation
#include "ADXL345_RealAccelHandler_impl.h"

#endif //__ADXL345_REAL_ACCEL_HANDLER_INTERFACE_H__