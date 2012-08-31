#ifndef __QUEUE_UNIT_TEST_IMPL_H__
#define __QUEUE_UNIT_TEST_IMPL_H__

#include "..\Containers\Queue.h"

#ifndef __QUEUE_UNIT_TEST_H__
    #error Must be included from QueueUnitTest.h
#endif

void QueueUnitTest::Setup()
{

}

void QueueUnitTest::RunTestCases()
{
    TestCaseEnqueueEmpty();
    TestCaseEnqueueOne();
    TestCaseEnqueueMoreThanOne();
    TestCaseDequeueEmpty();
}

// Enqueue on Empty
void QueueUnitTest::TestCaseEnqueueEmpty()
{
    Queue<int> _Queue;
    
    // Verify Queue is Empty
    TEST( _Queue.Empty() );
    
    // Enqueue
    _Queue.Enqueue(10);
    
    // Verify not empty
    TEST( !_Queue.Empty() );
    
    // Dequeue
    TEST( 10 == _Queue.Dequeue() );
    
    // Verify Empty
    TEST( _Queue.Empty() );
    
}

// Enqueue on one element
void QueueUnitTest::TestCaseEnqueueOne()
{
    Queue<int> _Queue;
    
    // Enqueue
    _Queue.Enqueue(12);
    _Queue.Enqueue(15);
    
    TEST( 12 == _Queue.Dequeue() );
    TEST( 15 == _Queue.Dequeue() );
    TEST( _Queue.Empty() );
}

// Enqueue on more than one element
void QueueUnitTest::TestCaseEnqueueMoreThanOne()
{
    Queue<int> _Queue;
    
    for( int i = 0; i < 30; ++i )
    {
        _Queue.Enqueue(i);
    }
    
    for( int i = 0; i < 30; ++i )
    {
        TEST( i == _Queue.Dequeue() );
    }
    
    TEST( _Queue.Empty() );
    
    for( int i = 30; i > 0; --i )
    {
        _Queue.Enqueue(i);
    }
    
    for( int i = 30; i > 0; --i )
    {
        TEST( i == _Queue.Dequeue() );
    }
}

// Dequeue on Empty
void QueueUnitTest::TestCaseDequeueEmpty()
{
    Queue<int> _Queue;
    
    TEST( int() == _Queue.Dequeue() );
}

#endif //__QUEUE_UNIT_TEST_H__