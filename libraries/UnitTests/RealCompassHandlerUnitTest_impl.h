#ifndef __RCHUT_IMPL_H__
#define __RCHUT_IMPL_H__

#ifndef __RCHUT_H__
    #error Must be included from RealCompassHandlerUnitTest.h
#endif

#include "..\Compass\RealCompassHandler.h"
#include <HardwareSerial.h>

RealCompassHandlerUnitTest::RealCompassHandlerUnitTest()
    : 
     UnitTest()
    ,SimulatedDeviceController()
    ,mReceiveBuffer()
    ,mComPort(0x19, *this) // device id is irrelevant for compass test
    ,mCompass( mComPort )
    ,mCompassData()
{ 
    // setup compass data
    mCompassData = 1.00, 2.00, 3.00;
    
    // setup output pipe
    mCompass.AttachOutputLine( mCompassData );
    
}

void RealCompassHandlerUnitTest::Setup()
{

}

void RealCompassHandlerUnitTest::RunTestCases()
{
    TestCommands();
    TestDeviceConfig();
    TestMeasurementRate();
}

// Define Macro for Testing Commands:
#define COMPASS_TestCommand(name,result) \
{ \
    mCompass.name(); \
    uint8_t name##_cmd = mReceiveBuffer.Dequeue(); \
    while( !mReceiveBuffer.Empty() ){ mReceiveBuffer.Dequeue(); } \
    TEST( name##_cmd == result ); \
}

void RealCompassHandlerUnitTest::TestCommands()
{ 
    // Test Commands:
    float arr[] = { 0.0, 0.0, 0.0 };
    
    COMPASS_TestCommand(GetData,0x50);
    TEST( mCompassData == arr );
    COMPASS_TestCommand(SetOrientationLevel,0x72);
    COMPASS_TestCommand(SetOrientationUprightEdge,0x73);
    COMPASS_TestCommand(SetOrientationUprightFront,0x74);
    COMPASS_TestCommand(EnterRunMode,0x75);
    COMPASS_TestCommand(EnterStandbyMode,0x76);
    COMPASS_TestCommand(EnterCalibrationMode,0x71);
    COMPASS_TestCommand(ExitCalibrationMode,0x7E);
    COMPASS_TestCommand(ResetDevice,0x82);
    COMPASS_TestCommand(EnterSleepMode,0x83);
    COMPASS_TestCommand(ExitSleepMode,0x84);
}

void RealCompassHandlerUnitTest::TestDeviceConfig()
{
    mCompass.GetConfig();
    
    // Clock back data 
    uint8_t GetConfig_cmd = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ) { mReceiveBuffer.Dequeue(); }
    
    TEST( GetConfig_cmd == 0x65 );
    
    // Test Eeprom Write
    CompassConfig _config;
    mCompass.SetConfig( _config );
    
    uint8_t eepWrite = mReceiveBuffer.Dequeue();
    uint8_t SetConfig_eeprom = mReceiveBuffer.Dequeue();
    uint8_t SetConfig_data = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ) { mReceiveBuffer.Dequeue(); }
    
    TEST( eepWrite == 0xF1 );
    TEST( SetConfig_eeprom == 0x04 );
    TEST( SetConfig_data == _config.GetValue() );
}

void RealCompassHandlerUnitTest::TestMeasurementRate()
{ 
    // Test Eeprom Read
    mCompass.GetMeasurementRate( );
    
    uint8_t eepRead = mReceiveBuffer.Dequeue();
    uint8_t GetConfig_eeprom = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ) { mReceiveBuffer.Dequeue(); }
    
    TEST( eepRead == 0xE1 );
    TEST( GetConfig_eeprom == 0x05 );
    
    // Test Eeprom Write
    CompassRate _rate;
    mCompass.SetMeasurementRate( _rate.GetDeviceRate() );
    
    uint8_t eepWrite = mReceiveBuffer.Dequeue();
    uint8_t SetConfig_eeprom = mReceiveBuffer.Dequeue();
    uint8_t SetConfig_data = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ) { mReceiveBuffer.Dequeue(); }
    
    TEST( eepWrite == 0xF1 );
    TEST( SetConfig_eeprom == 0x05 );
    TEST( SetConfig_data == _rate.GetValue() );
}

void RealCompassHandlerUnitTest::DataAvailable( SimulatedDevice& device )
{
    // Get Data
    while( device.DataAvailable() )
    {
        mReceiveBuffer.Enqueue( device.ReadData() );
    }
    
    // no need to write response since 
    // responses are discarded anyways.
}

#endif //__RCHUT_IMPL_H__