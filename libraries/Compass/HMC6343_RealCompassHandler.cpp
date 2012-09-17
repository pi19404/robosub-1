#ifndef __REAL_COMPASS_HANDLER_CPP__
#define __REAL_COMPASS_HANDLER_CPP__

#include "RealCompassHandler.h"
#include <ComPort/ComPort.h>

RealCompassHandler::RealCompassHandler(ComPort& comPort)
    :
     RealCompassHandlerInterface<data_type>()
    ,mCompassData()
    ,mDeviceConfig()
    ,mDeviceRate()
    ,mComPort(comPort)
{ 

}

RealCompassHandler::~RealCompassHandler()
{

}

// From DeviceHandler
void RealCompassHandler::Initialize()
{
    // Initialize Data
    GetDeviceConfig();
    GetDeviceRate();
    GetData();
}

void RealCompassHandler::GetData()
{
    CompassData<data_type> _Data;
    
    DoGetData();
    
    _Data = mCompassData;
    _Data /= data_type(10.0);

    // update pipes
    for( size_t i = 0; i < mOPipes.Size(); ++i )
    {
        mOPipes[i].WriteTo( _Data );
    }
}

void RealCompassHandler::SetOrientationLevel( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::SetOrientationLevel ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::SetOrientationUprightEdge( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::SetOrientationUprightEdge ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::SetOrientationUprightFront( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::SetOrientationUprightFront ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::EnterRunMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::EnterRunMode ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::EnterStandbyMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::EnterStandbyMode ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::EnterCalibrationMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::EnterCalibrationMode ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::ExitCalibrationMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::ExitCalibrationMode ].Addr;
    DoSendCommand(addr);
    
    // Wait for device to exit calibration mode
    delay(50);
}

void RealCompassHandler::ResetDevice( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::ResetDevice ].Addr;
    DoSendCommand(addr);
    
    // Wait for device to reset
    delay(500);
}

void RealCompassHandler::EnterSleepMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::EnterSleepMode ].Addr;
    DoSendCommand(addr);
}

void RealCompassHandler::ExitSleepMode( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::ExitSleepMode ].Addr;
    DoSendCommand(addr);
    
    // Wait for device to exit sleep mode
    delay(20);
}

// Additional Methods
void RealCompassHandler::SetConfig( CompassConfig& config )
{
    mDeviceConfig = config;
    SetDeviceConfig();
}
    
CompassConfig RealCompassHandler::GetConfig( void )
{
    GetDeviceConfig();
    return mDeviceConfig;
}

CompassRate::Enum RealCompassHandler::GetMeasurementRate( void )
{
    GetDeviceRate();
    return mDeviceRate.GetDeviceRate();
}

void RealCompassHandler::SetMeasurementRate( CompassRate::Enum rate )
{
    mDeviceRate = rate;
    SetDeviceRate();
}

void RealCompassHandler::EepromWrite( uint8_t addr, uint8_t data )
{
    uint8_t eepromAddr = CompassRegStats [ CompassRegNames::EepromWrite ].Addr;
    uint8_t args[] = { eepromAddr, addr, data };
    mComPort.write( args, 3 );
}

uint8_t RealCompassHandler::EepromRead( uint8_t addr )
{
    uint8_t eepromAddr = CompassRegStats [ CompassRegNames::EepromRead ].Addr;
    uint8_t sendData[] = { eepromAddr, addr };
    uint8_t recvData;
    mComPort.read( sendData, 2, &recvData, 1 );
    return recvData;
}

void RealCompassHandler::DoSendCommand( uint8_t addr )
{
    mComPort.write( &addr, 1 );
}

void RealCompassHandler::SetDeviceConfig( void )
{
    EepromWrite( CompassEepromStats [ CompassEepromRegNames::Config ].Addr, mDeviceConfig.GetValue() );
}

void RealCompassHandler::GetDeviceConfig( void )
{
    uint8_t addr = CompassRegStats [ CompassRegNames::GetConfig ].Addr;
    uint8_t config;
    mComPort.read( &addr, 1, &config, 1 );
    mDeviceConfig = config;
}

void RealCompassHandler::SetDeviceRate( )
{
    EepromWrite( CompassEepromStats [ CompassEepromRegNames::Rate ].Addr, mDeviceRate.GetValue() );
}
    
void RealCompassHandler::GetDeviceRate( void )
{
    mDeviceRate = EepromRead( CompassEepromStats [ CompassEepromRegNames::Rate ].Addr );
}

void RealCompassHandler::DoGetData( )
{
    // allocate temporary storage
    uint8_t data[6];
    
    // read data from device
    uint8_t addr = CompassRegStats[ CompassRegNames::GetHeadingData ].Addr;
    
    mComPort.read( &addr, 1, data, 6 );
    
    // data is MSB | LSB, place in vector
    for( unsigned i = 0; i < 3; ++i )
    {     
        unsigned j = 2*i;
        mCompassData[i]  = (int16_t)(data[j]) << 8;
        mCompassData[i] += (int16_t)(data[j+1]) & 0xff;
    }
}

#endif //__REAL_COMPASS_HANDLER_CPP__
