// Verify ADXL345 is being used
#ifdef ACL && ACL == ADXL345

#include "RealAccelHandler.h"
#include "../ComPort/ComPort.h"

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

#endif //(ACCEL) && (ACCEL == ADXL345)
