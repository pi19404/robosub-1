#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__

#include "TestManager.h"

class UnitTest
{
    public:
        UnitTest() : mTestManager(TestManager::GetInstance()) { }
        
        virtual void Setup()        = 0;
        virtual void RunTestCases() = 0;
        
    protected:
        void Pass( const char* str )
        {
            mTestManager.Pass(str);
        }
        void Fail( const char* str )
        {
            mTestManager.Fail(str);
        }
        
    private:
        TestManager& mTestManager;
};

#define STR(x) #x
#define TO_STR(x) STR(x)
#define AT " File: " __FILE__ " Line: " TO_STR(__LINE__)

// Use TEST(x) to log test cases
#define TEST(x) ( (x) ? Pass(#x) : Fail(#x AT) )

#endif //__UNIT_TEST_H__
