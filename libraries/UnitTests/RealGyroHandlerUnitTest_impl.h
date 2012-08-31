#ifndef __RGHUT_IMPL_H__
#define __RGHUT_IMPL_H__

#ifndef __RGHUT_H__
    #error Must be included from RealGyroHandlerUnitTest.h
#endif

#include "..\Gyro\RealGyroHandler.h"

RealGyroHandlerUnitTest::RealGyroHandlerUnitTest()
    :
     UnitTest()
    ,SimulatedDeviceController()
    ,mReceiveBuffer()
    ,mComPort(0x69, *this) // device id is not important for this unit test
    ,mGyro( mComPort )
{

}

RealGyroHandlerUnitTest::~RealGyroHandlerUnitTest()
{ 

}

void RealGyroHandlerUnitTest::Setup()
{

}

void RealGyroHandlerUnitTest::RunTestCases()
{
    TestCaseGetData();
    TestCaseSetCtrlReg1();
    TestCaseSetCtrlReg2();
    TestCaseSetCtrlReg3();
    TestCaseSetCtrlReg4();
    TestCaseSetCtrlReg5();
    TestCaseGetStatus();
    TestCaseSetFifoCtrlReg();
    TestCaseGetFifoStatus();
    TestCaseSetInt1Config();
    TestCaseGetInt1Status();
    TestCaseSetThresholdX();
    TestCaseSetThresholdY();
    TestCaseSetThresholdZ();
    TestCaseSetInt1Duration();
}

void RealGyroHandlerUnitTest::TestCaseGetData()
{
    mGyro.GetData();
    uint8_t cmds[] = { 0x29, 0x28, 0x2B, 0x2A, 0x2D, 0x2C };
    
    for( size_t i = 0; i < sizeof(cmds); ++i )
    {
        uint8_t data_cmd = mReceiveBuffer.Dequeue();
        TEST( data_cmd == cmds[i] );
    }
}

void RealGyroHandlerUnitTest::TestCaseSetCtrlReg1()
{
    GyroCtrlReg1 cr1;
    mGyro.SetCtrlReg1( cr1 );
    
    // Check buffer
    uint8_t cr1_cmd = mReceiveBuffer.Dequeue();
    
    TEST( cr1_cmd == 0x20 );
}

void RealGyroHandlerUnitTest::TestCaseSetCtrlReg2()
{
    GyroCtrlReg2 cr2;
    mGyro.SetCtrlReg2( cr2 );
    
    // Check buffer
    uint8_t cr2_cmd = mReceiveBuffer.Dequeue();
    
    TEST( cr2_cmd == 0x21 );
}

void RealGyroHandlerUnitTest::TestCaseSetCtrlReg3()
{
    GyroCtrlReg3 cr3;
    mGyro.SetCtrlReg3( cr3 );
    
    // Check buffer
    uint8_t cr3_cmd = mReceiveBuffer.Dequeue();
    
    TEST( cr3_cmd == 0x22 );
}

void RealGyroHandlerUnitTest::TestCaseSetCtrlReg4()
{
    GyroCtrlReg4 cr4;
    mGyro.SetCtrlReg4( cr4 );
    
    // Check buffer
    uint8_t cr4_cmd = mReceiveBuffer.Dequeue();
    
    TEST( cr4_cmd == 0x23 );
}

void RealGyroHandlerUnitTest::TestCaseSetCtrlReg5()
{
    GyroCtrlReg5 cr5;
    mGyro.SetCtrlReg5( cr5 );
    
    // Check buffer
    uint8_t cr5_cmd = mReceiveBuffer.Dequeue();
    
    TEST( cr5_cmd == 0x24 );
}

void RealGyroHandlerUnitTest::TestCaseGetStatus()
{
    mGyro.GetStatus();
    
    uint8_t status_cmd = mReceiveBuffer.Dequeue();
    TEST( status_cmd == 0x27 );
}

void RealGyroHandlerUnitTest::TestCaseSetFifoCtrlReg()
{
    GyroFifoCtrlReg fcr;
    
    mGyro.SetFifoCtrlReg(fcr);
    
    uint8_t fifoctrl_cmd = mReceiveBuffer.Dequeue();
    TEST( fifoctrl_cmd == 0x2E );
}

void RealGyroHandlerUnitTest::TestCaseGetFifoStatus()
{
    mGyro.GetFifoStatus();
    
    uint8_t fifostatus_cmd = mReceiveBuffer.Dequeue();
    TEST( fifostatus_cmd == 0x2F );
}

void RealGyroHandlerUnitTest::TestCaseSetInt1Config()
{
    GyroInt1CfgReg icr;
    mGyro.SetInt1Config( icr );
    
    uint8_t int1cfg_cmd = mReceiveBuffer.Dequeue();
    TEST( int1cfg_cmd == 0x30 );
}

void RealGyroHandlerUnitTest::TestCaseGetInt1Status()
{
    mGyro.GetInt1Status();
    
    uint8_t int1sts_cmd = mReceiveBuffer.Dequeue();
    TEST( int1sts_cmd == 0x31 );
}

void RealGyroHandlerUnitTest::TestCaseSetThresholdX()
{
    mGyro.SetThresholdX( 0xFF );
    uint8_t threshxh = mReceiveBuffer.Dequeue();
    uint8_t threshxl = mReceiveBuffer.Dequeue();
    TEST( threshxh == 0x32 );
    TEST( threshxl == 0x33 );
}

void RealGyroHandlerUnitTest::TestCaseSetThresholdY()
{
    mGyro.SetThresholdY( 0xFF );
    uint8_t threshyh = mReceiveBuffer.Dequeue();
    uint8_t threshyl = mReceiveBuffer.Dequeue();
    TEST( threshyh == 0x34 );
    TEST( threshyl == 0x35 );
}

void RealGyroHandlerUnitTest::TestCaseSetThresholdZ()
{
    mGyro.SetThresholdZ( 0xFF );
    uint8_t threshzh = mReceiveBuffer.Dequeue();
    uint8_t threshzl = mReceiveBuffer.Dequeue();
    TEST( threshzh == 0x36 );
    TEST( threshzl == 0x37 );
}

void RealGyroHandlerUnitTest::TestCaseSetInt1Duration()
{
    GyroInt1Duration idur;
    mGyro.SetInt1Duration( idur );
    uint8_t idur_cmd = mReceiveBuffer.Dequeue();
    TEST( idur_cmd == 0x38 );
}


void RealGyroHandlerUnitTest::DataAvailable( SimulatedDevice& device )
{
    // clock out command, don't care about rest.
    mReceiveBuffer.Enqueue( device.ReadData() );
    
    while( device.DataAvailable() )
    {
        // clear port buffer
        device.ReadData();
    }
    
    // Don't bother with a response
}

#endif //__RMCHUT_IMPL_H__