#ifndef __ADXL345_REAL_ACCEL_HANDLER_IMPL_H__
#define __ADXL345_REAL_ACCEL_HANDLER_IMPL_H__

#ifndef __ADXL345_REAL_ACCEL_HANDLER_INTERFACE_H__
    #error Must be included from ADXL345_RealAccelHandler.h
#endif

#include "../ComPort/ComPort.h"

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

RealAccelHandler::RealAccelHandler( ComPort& comPort )
    :
     mComPort(comPort)
    ,mAccelData()
{

}

        
void RealAccelHandler::Initialize()
{
    SetDataRatePwrModeCtrl( 
            LowPower::No            // Low power mode is for sissies ;-)
        |   DataRate::_100Hz        // Set ODR to 100Hz
    );
    
    SetPwrCtrl( 
            ActInactMode::Separate
        |   AutoSleep::No
        |   MeasurementMode::Run    // Enable the Device
        |   SleepMode::StandbyRun
        |   WakeupFrequency::_8Hz
    );
    
    SetIntCtrl( 
            Interrupts::None        // All Interrupts Off
    );
    
    SetIntMap( 
            InterruptMap::Int1      // All Interrupts Trigger in Int1
    );
    
    SetDataFormat( 
            TestForce::Off
        |   SpiMode::_4Wire
        |   InterruptInversion::No
        |   FullResolutionMode::Yes     // All output values are scaled by 256
        |   Justify::Right              // LSB first
        |   MeasurementRange::_2g       // +/- 2g measurement
    );
    
    SetFifoCtrl( 
            FifoMode::Bypass
        |   Trigger::Int1
    );
    
    // Do First Read
    GetData();
}

void RealAccelHandler::GetData()
{
    AccelData<data_type> _Data;
    
    DoGetData();
    
    _Data = mAccelData;
    _Data /= 256;           // Scale Value for FullScale Resolution Mode
    
    for( size_t i = 0; i < mOPipes.Size(); ++i )
    {
        mOPipes[i].WriteTo( _Data );
    }
}

void RealAccelHandler::SetXOffsetInMillig( data_type offset )
{
    using namespace RegNames;
    WriteReg( OffsetX, ACCEL_OFFSET_mG_TO_VAL(offset) );
}

void RealAccelHandler::SetYOffsetInMillig( data_type offset )
{
    using namespace RegNames;
    WriteReg( OffsetY, ACCEL_OFFSET_mG_TO_VAL(offset) );
}

void RealAccelHandler::SetZOffsetInMillig( data_type offset )
{
    using namespace RegNames;
    WriteReg( OffsetZ, ACCEL_OFFSET_mG_TO_VAL(offset) );
}

void RealAccelHandler::SetTapDurationInMilliseconds( data_type dur )
{
    using namespace RegNames;
    WriteReg( TapDuration, ACCEL_TAP_DUR_ms_TO_VAL(dur) );
}

void RealAccelHandler::SetLatencyInMiliseconds( data_type lat )       
{
    using namespace RegNames;
    WriteReg( TapLatency, ACCEL_TAP_LAT_ms_TO_VAL(lat) );
}

void RealAccelHandler::SetTapWindowInMilliseconds( data_type win )
{
    using namespace RegNames;
    WriteReg( TapWindow, ACCEL_TAP_WINDOW_ms_TO_VAL(win) );
}

void RealAccelHandler::SetTapThresholdInMillig( data_type thresh )
{
    using namespace RegNames;
    WriteReg( TapThreshold, ACCEL_TAP_THRESH_mG_TO_VAL(thresh) );
}

void RealAccelHandler::SetActivityThresholdInMillig( data_type thresh )
{
    using namespace RegNames;
    WriteReg( ActivityThreshold, ACCEL_ACT_THRESH_mG_TO_VAL(thresh) );
}

void RealAccelHandler::SetInactivityThresholdInMillig( data_type thresh )
{
    using namespace RegNames;
    WriteReg( InactivityThreshold, ACCEL_INACT_THRESH_mG_TO_VAL(thresh) );
}

void RealAccelHandler::SetInactivityTimeInSeconds( data_type time )
{
    using namespace RegNames;
    WriteReg( InactivityTime, ACCEL_TIME_INACT_s_TO_VAL(time) );
}

void RealAccelHandler::SetActInactCtrl( const ActInactCtrlReg& reg)
{
    using namespace RegNames;
    WriteReg( ActInactControl, reg.GetValue() );
}

void RealAccelHandler::SetFreeFallThresholdInMillig( data_type thresh )
{
    using namespace RegNames;
    WriteReg( FreeFallThreshold, ACCEL_FF_THRESH_mG_TO_VAL(thresh) );
}

void RealAccelHandler::SetFreeFallTimeInMilliseconds( data_type time )
{
    using namespace RegNames;
    WriteReg( FreeFallTime, ACCEL_FF_TIME_ms_TO_VAL(time) );
}

void RealAccelHandler::SetTapAxesControl( const TapAxesCtrlReg& reg ) 
{
    using namespace RegNames;
    WriteReg( TapAxesControl, reg.GetValue() );
}

const ActTapSrcReg& RealAccelHandler::GetActTapSrc()
{
    using namespace RegNames;
    return ReadReg( ActTapSrc );
}

void RealAccelHandler::SetDataRatePwrModeCtrl( const DataRatePwrModeCtrlReg& reg )
{
    using namespace RegNames;
    WriteReg( DataRatePwrModeCtrl, reg.GetValue() );
}

void RealAccelHandler::SetPwrCtrl( const PwrCtrlReg& reg )
{
    using namespace RegNames;
    WriteReg( PwrCtrl, reg.GetValue() );
}

void RealAccelHandler::SetIntCtrl( const IntCtrlReg& reg)
{
    using namespace RegNames;
    WriteReg( InterruptEnableControl, reg.GetValue() );
}

void RealAccelHandler::SetIntMap( const IntMapReg& reg)
{
    using namespace RegNames;
    WriteReg( InterruptMapControl, reg.GetValue() );
}

const IntSrcReg& RealAccelHandler::GetIntSrc()
{
    using namespace RegNames;
    return ReadReg( InterruptSource );
}

void RealAccelHandler::SetDataFormat( const DataFormatReg& reg)
{
    using namespace RegNames;
    WriteReg( DataFormat, reg.GetValue() );
}

void RealAccelHandler::SetFifoCtrl( const FifoCtrlReg& reg)
{
    using namespace RegNames;
    WriteReg( FifoCtrl, reg.GetValue() );
}

const FifoStatusReg& RealAccelHandler::GetFifoStatus()
{
    using namespace RegNames;
    return ReadReg( FifoStatus );
}

void RealAccelHandler::WriteReg( RegNames::Enum en, uint8_t data )
{
    uint8_t sendData[] = { RegStats[ en ].Addr, data };
    mComPort.write( sendData, 2 ); 
}

uint8_t RealAccelHandler::ReadReg( RegNames::Enum en )
{
    uint8_t data;
    uint8_t addr = RegStats[ en ].Addr;
    mComPort.read( &addr, 1, &data, 1 );
    return data;
}

void RealAccelHandler::DoGetData()
{
    using namespace RegNames;
    
    mAccelData[0] = (int16_t)( (ReadReg(DataXH) << 8) | ReadReg(DataXL) );
    mAccelData[1] = (int16_t)( (ReadReg(DataYH) << 8) | ReadReg(DataYL) );
    mAccelData[2] = (int16_t)( (ReadReg(DataZH) << 8) | ReadReg(DataZL) );
}

#endif //__ADXL345_REAL_ACCEL_HANDLER_IMPL_H__
