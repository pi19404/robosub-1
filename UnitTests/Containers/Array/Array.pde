//
// Unit Test Program for Array Class
//
#include <TestManager.h>
#include <UnitTestRunner.h>
#include "ArrayUnitTest.h"

void setup()
{
  // Get Test Manager:
  TestManager&  testManager = TestManager::GetInstance();
  
  // Create Tests
  ArrayUnitTest  arrayUnitTest;
  UnitTestRunner arrayTestRunner( &arrayUnitTest );
  
  // Start Test Manager
  testManager.Run(5);
  
  // Run Tests
  arrayTestRunner.Run();
  testManager.LogResults();
  
}


void loop()
{
  
  
}
