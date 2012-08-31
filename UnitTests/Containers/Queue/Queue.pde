//
// Unit Test Program for Queue Class
//
#include <new.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include <QueueUnitTest.h>

void setup()
{
  // Get Test Manager:
  TestManager&  testManager = TestManager::GetInstance();
  
  // Create Tests
  QueueUnitTest  queueUnitTest;
  UnitTestRunner queueTestRunner( &queueUnitTest );
  
  // Start Test Manager
  testManager.Run(5);
  
  // Run Tests
  queueTestRunner.Run();
  testManager.LogResults();
  
}


void loop()
{
  
  
}
