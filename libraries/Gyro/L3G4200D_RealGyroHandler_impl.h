#ifndef __L3G4200D_REAL_GYRO_HANDLER_IMPL_H__
#define __L3G4200D_REAL_GYRO_HANDLER_IMPL_H__

#ifndef __L3G4200D_REAL_GYRO_HANDLER_INTERFACE_H__
    #error Must be included from L3G4200D_RealGyroHandlerInterface.h
#endif

#include "../ComPort/ComPort.h"

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

//---------------------------------
//---------------------------------
//-----BEGIN DEFINITIONS-----------
//---------------------------------
//---------------------------------

RealGyroHandler::RealGyroHandler( ComPort& comPort )
    :
     RealGyroHandlerInterface<data_type>()
    ,mGyroData()
    ,mComPort(comPort)
{

}

// From DeviceHandler
void RealGyroHandler::Initialize()
{        
    SetCtrlReg1(
                  GyroAxis::X | GyroAxis::Y | GyroAxis::Z   // Power X, Y, Z
                | GyroPowerMode::Normal                     // Power on Gyro 
                | GyroDataRate::_800Hz                      // Set Data Rate 800Hz
                | GyroBandwith::Three                       // FIXME: Magic value
    );
    
    SetCtrlReg2(
                  GyroHpfDivider::_12_5                     // default hpf
                | GyroHpfMode::NormalWithReset              // default hpf mode
    );
    
    SetCtrlReg3( 
                   GyroInterrupts::None                     // disable interrupts
    );
    
    SetCtrlReg4(
                   GyroBlockDataUpdate::Continuous          // Continuous stream of data
                |  GyroDataSelection::LittleEndian          // LSB then MSB
                |  GyroFullScaleSelection::_250dps          // +/- 250 degrees per second max reading
                |  GyroSelfTest::Disable                    // SelfTest Disable
                |  GyroSpiInterfaceMode::_4Wire             // We use I2C, set to default
    );
    
    SetCtrlReg5(
                   GyroRebootMemoryContent::No              // Don't store configuration through reboots
                |  GyroFifoEnable::No                       // Disable FIFO
                |  GyroHpfEnable::No                        // Disable High-Pass Filter
                |  GyroDataFifoFilterMode::Off              // Do not filter data for FIFO
                |  GyroIntSelFilterMode::Off                // Do not filter data for interrupts
    );
    
    uint8_t fifoThreshold = ( 0x1F & 0x00 );    // replace 0x00 with threshold value, when mode is enable
    SetFifoCtrlReg(
                    GyroFifoMode::Bypass        // Bypass Mode
                |   fifoThreshold               // Set threshold to above value
    );
                   
        
    // Do first data read
    GetData();
}

void RealGyroHandler::GetData()
{
    GyroData<data_type> _Data;
    
    DoGetData();
    
    _Data = mGyroData;
    _Data *= 8.75/1000.0; // convert to degrees per second
                          // value is LSB/8.75 millig at 250Hz Data Rate

    // update pipes
    for( size_t i = 0; i < mOPipes.Size(); ++i )
    {
        mOPipes[i].WriteTo( _Data );
    }
}

void RealGyroHandler::SetCtrlReg1( const GyroCtrlReg1& reg )
{
    WriteReg(GyroRegNames::CtrlReg1, reg.GetValue() );
}

void RealGyroHandler::SetCtrlReg2( const GyroCtrlReg2& reg )
{
    WriteReg(GyroRegNames::CtrlReg2, reg.GetValue() );
}

void RealGyroHandler::SetCtrlReg3( const GyroCtrlReg3& reg )
{
    WriteReg(GyroRegNames::CtrlReg3, reg.GetValue() );
}

void RealGyroHandler::SetCtrlReg4( const GyroCtrlReg4& reg )
{
    WriteReg(GyroRegNames::CtrlReg4, reg.GetValue() );
}

void RealGyroHandler::SetCtrlReg5( const GyroCtrlReg5& reg )
{
    WriteReg(GyroRegNames::CtrlReg5, reg.GetValue() );
}

const GyroStatusReg& RealGyroHandler::GetStatus()
{
    return ReadReg(GyroRegNames::StatusReg);
}

void RealGyroHandler::SetFifoCtrlReg( const GyroFifoCtrlReg& reg )
{
    WriteReg(GyroRegNames::FifoCtrlReg, reg.GetValue() );
}

const GyroFifoSrcReg& RealGyroHandler::GetFifoStatus()
{
    return ReadReg(GyroRegNames::FifoSrcReg);
}

void RealGyroHandler::SetInt1Config( const GyroInt1CfgReg& reg )
{
    WriteReg(GyroRegNames::Int1Cfg, reg.GetValue() );
}

const GyroInt1SrcReg& RealGyroHandler::GetInt1Status()
{
    return ReadReg(GyroRegNames::Int1Src);
}

void RealGyroHandler::SetThresholdX( uint16_t val )
{
    uint8_t XH = (val >>   8);
    uint8_t XL = (val & 0xff);
    
    WriteReg(GyroRegNames::Int1ThreshXH, XH);
    WriteReg(GyroRegNames::Int1ThreshXL, XL);
}

void RealGyroHandler::SetThresholdY( uint16_t val )
{
    uint8_t YH = (val >>   8);
    uint8_t YL = (val & 0xff);
    
    WriteReg(GyroRegNames::Int1ThreshYH, YH);
    WriteReg(GyroRegNames::Int1ThreshYL, YL);
}

void RealGyroHandler::SetThresholdZ( uint16_t val )
{
    uint8_t ZH = (val >>   8);
    uint8_t ZL = (val & 0xff);
    
    WriteReg(GyroRegNames::Int1ThreshZH, ZH);
    WriteReg(GyroRegNames::Int1ThreshZL, ZL);
}

void RealGyroHandler::SetInt1Duration( const GyroInt1Duration& reg )
{
    WriteReg(GyroRegNames::Int1Duration, reg.GetValue() );
}

void RealGyroHandler::WriteReg( GyroRegNames::Enum en, uint8_t val )
{
    uint8_t addr = GyroRegStats[(int)en].Addr;
    uint8_t data[] = { addr, val };
    mComPort.write(data, 2);
}

uint8_t RealGyroHandler::ReadReg( GyroRegNames::Enum en )
{
    uint8_t addr = GyroRegStats[(int)en].Addr;
    uint8_t data;
    mComPort.read(&addr, 1, &data, 1);
    return data;
}

void RealGyroHandler::DoGetData( )
{  
    using namespace GyroRegNames;
    
    // Get Data
    mGyroData[0]  = (int16_t)( (ReadReg(OutXH) << 8) | ReadReg(OutXL) );
    mGyroData[1]  = (int16_t)( (ReadReg(OutYH) << 8) | ReadReg(OutYL) );
    mGyroData[2]  = (int16_t)( (ReadReg(OutZH) << 8) | ReadReg(OutZL) );
}

#endif //__L3G4200D_REAL_GYRO_HANDLER_IMPL_H__
