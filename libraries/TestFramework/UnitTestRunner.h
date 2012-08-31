#ifndef __UNIT_TEST_RUNNER_H__
#define __UNIT_TEST_RUNNER_H__

#include "UnitTest.h"

class UnitTestRunner
{
    public:
        UnitTestRunner( UnitTest* unitTest = 0 ) : mUnitTest(unitTest) { }
        
        void Run()
        {
            if( mUnitTest )
            {
                mUnitTest->Setup();
                mUnitTest->RunTestCases();
            }
        }
        
    protected:
        UnitTest* mUnitTest;
};

#endif //__UNIT_TEST_RUNNER_H__