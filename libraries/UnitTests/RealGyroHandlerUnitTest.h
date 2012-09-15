#ifndef __RGHUT_H__
#define __RGHUT_H__

#define GYRO L3G4200D   //Use ST Microelectronics Gyroscope

#include "../Containers/Queue.h"
#include "../ComPort/SimulatedComPort.h"

#include "../TestFramework/UnitTest.h"
#include "../Device/SimulatedDeviceController.h"

#include "../Gyro/RealGyroHandler.h"

#include <stdint.h>

class RealGyroHandlerUnitTest
    :
     public UnitTest
    ,public SimulatedDeviceController
{
    public:
        RealGyroHandlerUnitTest();
        ~RealGyroHandlerUnitTest();
        
        // from UnitTest
        virtual void Setup();
        
        virtual void RunTestCases();
        
    // Test Cases
    private:
        void TestCaseGetData();
        void TestCaseSetCtrlReg1();
        void TestCaseSetCtrlReg2();
        void TestCaseSetCtrlReg3();
        void TestCaseSetCtrlReg4();
        void TestCaseSetCtrlReg5();
        void TestCaseGetStatus();
        void TestCaseSetFifoCtrlReg();
        void TestCaseGetFifoStatus();
        void TestCaseSetInt1Config();
        void TestCaseGetInt1Status();
        void TestCaseSetThresholdX();
        void TestCaseSetThresholdY();
        void TestCaseSetThresholdZ();
        void TestCaseSetInt1Duration();
        
    private:
        // from Simulated Device Controller:
        virtual void DataAvailable( SimulatedDevice& );
        
    private:
        Queue<uint8_t>   mReceiveBuffer;
        SimulatedComPort mComPort;
        RealGyroHandler  mGyro;
};

#include "RealGyroHandlerUnitTest_impl.h"

#endif //__RGHUT_H__
