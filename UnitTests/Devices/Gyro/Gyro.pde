#include <LogManager.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include <RealGyroHandlerUnitTest.h>

void setup()
{
  // Get Test Manager:
  LogManager& logManager = LogManager::GetInstance();
  TestManager&  testManager = TestManager::GetInstance();
  RealGyroHandlerUnitTest unitTest;
  
  testManager.Run(10);
  UnitTestRunner testRunner(&unitTest);
  testRunner.Run();
  
  // End Tests
  testManager.LogResults();
  
}


void loop()
{
  
  
}
