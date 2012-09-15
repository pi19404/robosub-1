#ifndef __TEST_MANAGER_H__
#define __TEST_MANAGER_H__

#include "../Utility/Singleton.h"
#include "../Logging/LogManager.h"
#include <String.h>

class TestManager
    :
    public Singleton<TestManager>
{
    public:
        TestManager()
            :
             mLogManager(LogManager::GetInstance())
            ,mTestCases(0)
            ,mPasses(0)
            ,mFailures(0)
        { }
        
        void Run( unsigned rate )
        { 
            mLogManager.Begin(rate); 
        }
        
        void LogResults()
        {
            mLogManager.Log("Total Tests:\t" + STRING(mTestCases));
            mLogManager.Log("Total Passes:\t" + STRING(mPasses));
            mLogManager.Log("Total Failures:\t" + STRING(mFailures));
        }
        
        int TotalPasses()   { return mPasses;    }
        int TotalFailures() { return mFailures;  }
        int TotalTestCases(){ return mTestCases; }
        
        void Pass( const STRING& test_case )
        {
            mLogManager.Log( "Pass: " + test_case );
            ++mTestCases;
            ++mPasses;
        }
        
        void Fail( const STRING& test_case )
        {
            mLogManager.Log( "Fail: " + test_case );
            ++mTestCases;
            ++mFailures;
        }
        
    private:
        LogManager& mLogManager;
        int         mTestCases;
        int         mPasses;
        int         mFailures;
};


#endif //__TEST_MANAGER_H__
