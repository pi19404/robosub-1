#ifndef __RMCHUT_IMPL_H__
#define __RMCHUT_IMPL_H__

#ifndef __RMCHUT_H__
    #error Must be included from RealMotorCtrlHandlerUnitTest.h
#endif

#include "../MotorCtrl/RealMotorCtrlHandler.h"

#define MCTRL_ADDR 0x45

RealMotorCtrlHandlerUnitTest::RealMotorCtrlHandlerUnitTest()
    :
     UnitTest()
    ,SimulatedDeviceController()
    ,mReceiveBuffer()
    ,mComPort(MCTRL_ADDR, *this) // device id is important for motor controller test
    ,mController( mComPort )
{

}

RealMotorCtrlHandlerUnitTest::~RealMotorCtrlHandlerUnitTest()
{ 

}

void RealMotorCtrlHandlerUnitTest::Setup()
{

}

void RealMotorCtrlHandlerUnitTest::RunTestCases()
{
     TestCaseDriveMotor();
     TestCaseDriveMotorLowRes();
     TestCaseSetMinVoltage();
     TestCaseSetMaxVoltage();
}

void RealMotorCtrlHandlerUnitTest::TestCaseDriveMotor()
{
 
    Motor::Enum channel = Motor::One;
    MotorDirection::Enum dir = MotorDirection::Forward;
    uint8_t sendcommand = (uint8_t)channel + (uint8_t)dir;
    int8_t velocity = 124;
    uint16_t checksum = (MCTRL_ADDR +  sendcommand + velocity )&127; 
    mController.DriveMotor( channel, dir, velocity );
    
    // Get Response
    uint8_t addr = mReceiveBuffer.Dequeue();
    uint8_t cmd = mReceiveBuffer.Dequeue();
    uint8_t data = mReceiveBuffer.Dequeue();
    uint8_t recvChecksum = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ){ mReceiveBuffer.Dequeue(); }
    
    TEST( addr == MCTRL_ADDR );
    TEST( cmd == sendcommand );
    TEST( data == velocity );
    TEST( recvChecksum == (uint8_t)checksum );
}

void RealMotorCtrlHandlerUnitTest::TestCaseDriveMotorLowRes()
{

    LowResMotor::Enum motor = LowResMotor::One;
    uint8_t speedDir = 72; 
    uint16_t checksum = (MCTRL_ADDR + (uint8_t)motor + speedDir )&127;
    
    mController.DriveMotorLowRes( motor, speedDir );
    
    // Get Response
    uint8_t addr = mReceiveBuffer.Dequeue();
    uint8_t cmd = mReceiveBuffer.Dequeue();
    uint8_t data = mReceiveBuffer.Dequeue();
    uint8_t recvChecksum = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ){ mReceiveBuffer.Dequeue(); }
    
    TEST( addr == MCTRL_ADDR );
    TEST( cmd == (uint8_t)motor );
    TEST( data == speedDir );
    TEST( recvChecksum == (uint8_t)checksum );
}

void RealMotorCtrlHandlerUnitTest::TestCaseSetMinVoltage()
{

    uint8_t volcmd = 0x02;
    float voltage = 12.5;
    uint8_t volval = (uint8_t)(( voltage - 6 )*5);
    uint16_t checksum = (MCTRL_ADDR + volcmd + volval )&127;
    
    mController.SetMinVoltage( voltage );
    
    // Get Response
    uint8_t addr = mReceiveBuffer.Dequeue();
    uint8_t cmd = mReceiveBuffer.Dequeue();
    uint8_t data = mReceiveBuffer.Dequeue();
    uint8_t recvChecksum = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ){ mReceiveBuffer.Dequeue(); }
    
    TEST( addr == MCTRL_ADDR );
    TEST( cmd == volcmd );
    TEST( data == volval );
    TEST( recvChecksum == (uint8_t)checksum );
}

void RealMotorCtrlHandlerUnitTest::TestCaseSetMaxVoltage()
{

    uint8_t volcmd = 0x03;
    float voltage = 24.0;
    uint8_t volval = (uint8_t)(voltage * 5.12);
    uint16_t checksum = (MCTRL_ADDR + volcmd + volval )&127;
    
    mController.SetMaxVoltage( voltage );
    
    // Get Response
    uint8_t addr = mReceiveBuffer.Dequeue();
    uint8_t cmd = mReceiveBuffer.Dequeue();
    uint8_t data = mReceiveBuffer.Dequeue();
    uint8_t recvChecksum = mReceiveBuffer.Dequeue();
    while( !mReceiveBuffer.Empty() ){ mReceiveBuffer.Dequeue(); }
    
    TEST( addr == MCTRL_ADDR );
    TEST( cmd == volcmd );
    TEST( data == volval );
    TEST( recvChecksum == (uint8_t)checksum );
}

void RealMotorCtrlHandlerUnitTest::DataAvailable( SimulatedDevice& device )
{
    while( device.DataAvailable() )
    {
        // clock out data
        mReceiveBuffer.Enqueue( device.ReadData() );
    }
    
    // Don't bother with a response
}

#endif //__RMCHUT_IMPL_H__
