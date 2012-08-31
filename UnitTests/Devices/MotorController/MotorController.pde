#include <new.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include <RealMotorCtrlHandler.h>
#include <SimulatedComPort.h>
#include "RealMotorCtrlHandlerUnitTest.h"

void setup()
{
    TestManager& testManager = TestManager::GetInstance();
    testManager.Run(9600);
    
    
    RealMotorCtrlHandlerUnitTest controllerTest;
    UnitTestRunner controllerTestRunner( &controllerTest );
    
    controllerTestRunner.Run();
    testManager.LogResults();
}

void loop() { }
