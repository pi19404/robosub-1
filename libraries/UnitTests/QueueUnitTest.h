#ifndef __QUEUE_UNIT_TEST_H__
#define __QUEUE_UNIT_TEST_H__

#include "../TestFramework/UnitTest.h"

class QueueUnitTest
    :
    public UnitTest
{
    public:
        virtual void Setup();
        
        virtual void RunTestCases();
        
    // Test Cases:
    private:
        
        // Enqueue on Empty
        void TestCaseEnqueueEmpty();
        
        // Enqueue on one element
        void TestCaseEnqueueOne();
        
        // Enqueue on more than one element
        void TestCaseEnqueueMoreThanOne();
        
        // Dequeue on Empty
        void TestCaseDequeueEmpty();
};

#include "QueueUnitTest_impl.h"

#endif //__QUEUE_UNIT_TEST_H__
