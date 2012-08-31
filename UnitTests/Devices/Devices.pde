#include <LogManager.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include <DeviceHandlersUnitTest.h>

UnitTestRunner _Runner;
DeviceHandlersUnitTest _Test;
void setup()
{
  LogManager& _LogManager = LogManager::GetInstance();
  TestManager& _TestManager = TestManager::GetInstance();
  _TestManager.Run(10);
  _Runner = &_Test;
  _Runner.Run();
  
  // End Tests
  _TestManager.LogResults();
}


void loop()
{
  
  
}
