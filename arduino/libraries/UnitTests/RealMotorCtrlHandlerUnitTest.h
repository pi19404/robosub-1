#ifndef __RMCHUT_H__
#define __RMCHUT_H__

#include "../Containers/Queue.h"
#include "../ComPort/SimulatedComPort.h"

#include "../TestFramework/UnitTest.h"
#include "../Device/SimulatedDeviceController.h"

#include "../MotorCtrl/RealMotorCtrlHandler.h"

#include <stdint.h>

class RealMotorCtrlHandlerUnitTest
    :
     public UnitTest
    ,public SimulatedDeviceController
{
    public:
        RealMotorCtrlHandlerUnitTest();
        ~RealMotorCtrlHandlerUnitTest();
        
        // from UnitTest
        virtual void Setup();
        
        virtual void RunTestCases();
        
    // Test Cases
    private:
        void TestCaseDriveMotor();
        void TestCaseDriveMotorLowRes();
        void TestCaseSetMinVoltage();
        void TestCaseSetMaxVoltage();
        
    private:
        // from Simulated Device Controller:
        virtual void DataAvailable( SimulatedDevice& );
        
    private:
        Queue<uint8_t>          mReceiveBuffer;
        SimulatedComPort        mComPort;
        RealMotorCtrlHandler    mController;
};

#include "RealMotorCtrlHandlerUnitTest_impl.h"

#endif //__RMCHUT_H__
