//
// Unit Test Program for Containers Class
//
#include <LogManager.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include <ArrayUnitTest.h>
#include <QueueUnitTest.h>

void setup()
{
  // Get Test Manager:
  LogManager& logManager = LogManager::GetInstance();
  TestManager&  testManager = TestManager::GetInstance();
  UnitTestRunner testRunner;
  UnitTest* test;
  
  testManager.Run(10);
 
  // Run Queue Unit Test
  logManager.Log("----------QUEUE TEST----------");
  test = new QueueUnitTest;
  testRunner = test;
  testRunner.Run();
  delete test;
  
  // Run Array Unit Test
  logManager.Log("----------ARRAY TEST----------");
  test = new ArrayUnitTest;
  testRunner = test;
  testRunner.Run();
  delete test;
  
  
  // End Tests
  testManager.LogResults();
  
}


void loop()
{
  
  
}
