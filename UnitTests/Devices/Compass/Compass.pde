#include <new.h>
#include <TestManager.h>
#include <UnitTestRunner.h>
#include "RealCompassHandlerUnitTest.h"

void setup()
{
    TestManager& testManager = TestManager::GetInstance();
    testManager.Run(9600);
    
    
    RealCompassHandlerUnitTest compassTest;
    UnitTestRunner compassTestRunner( &compassTest );
    
    compassTestRunner.Run();
    testManager.LogResults();
}

void loop() { }
