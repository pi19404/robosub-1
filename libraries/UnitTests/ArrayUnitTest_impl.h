#ifndef __ARRAY_UNIT_TEST_IMPL_H__
#define __ARRAY_UNIT_TEST_IMPL_H__

#include "../Containers/Array.h"

#ifndef __ARRAY_UNIT_TEST_H__
    #error Must include from ArrayUnitTest.h
#endif

void ArrayUnitTest::Setup()
{

}

void ArrayUnitTest::RunTestCases()
{
    TestCaseDefaultConstructor();
    TestCaseEmptyArray();
    TestCaseCopyConstructor();
    //TestCaseSelfAssignment();
    TestCaseArrayTooLarge();
    TestCaseIndexOutsideBounds();
    TestCaseIndexDeletedElement();
}

// Default Constructor
void ArrayUnitTest::TestCaseDefaultConstructor()
{
    // Use Default Constructor:
    Array<int> _Array;
    
    // Verify Size is 0
    TEST( 0 == _Array.Size() );
    
    // Push 10
    _Array.PushBack(10);
    
    // Verify Size is 1
    TEST( 1 == _Array.Size() );
    
    // Verify that 10 is on top
    TEST( 10 == _Array.PopBack() );
    
    // Try to pop at 0
    TEST( int() == _Array.PopBack() );
}

// Create Empty Array
void ArrayUnitTest::TestCaseEmptyArray()
{
    Array<int> _Array(0);
    
    // Verify size is 0
    TEST( 0 == _Array.Size() );
    
    // Verify push is avaiable
    _Array.PushBack(10);
    _Array.PushBack(20);
    
    // Pop Reverse Order
    TEST( 20 == _Array.PopBack() );
    TEST( 10 == _Array.PopBack() );
}

// Copy Constructor
void ArrayUnitTest::TestCaseCopyConstructor()
{
    Array<int> _Array;
    
    for( size_t i = 0; i < 10; ++i )
    {
        _Array.PushBack(i);
    }
    
    Array<int> _Copy( _Array );
    
    // Verify identical size
    TEST( _Copy.Size() == _Array.Size() );
    
    // Verify same elements
    for( size_t i = 0; i < _Copy.Size(); ++i )
    {
        TEST( _Array[i] == _Copy[i] );
    }   
}

// Self Assignment
void ArrayUnitTest::TestCaseSelfAssignment()
{
    Array<int> _Array;
    _Array.PushBack(1);
    _Array.PushBack(2);
    
    _Array = _Array;
    
    TEST( _Array[0] == 1 );
    TEST( _Array[1] == 2 );
}

// Create Array too large
void ArrayUnitTest::TestCaseArrayTooLarge()
{
    Array<int> _Array(8192);
    
    // Verify array created
    TEST( 0 != _Array.MaxSize() );
    
    // Verify just smaller size
    TEST( _Array.Size() < 8192 );
}

// Index outide of array bounds
void ArrayUnitTest::TestCaseIndexOutsideBounds()
{
    Array<int> _Array;
    
    int i;
    for( i = 0; i < 30; ++i )
    {
        _Array.PushBack(i);
    }
    
    TEST( _Array[i] == (i-1) );
    TEST( _Array[i-2] == (i-2) );
}

// Index deleted element
void ArrayUnitTest::TestCaseIndexDeletedElement()
{
    Array<int> _Array;
    
    int i;
    for( i = 0; i < 30; ++i )
    {
        _Array.PushBack(i);
    }
    
    _Array.PopBack();
    
    TEST( _Array[i-1] != (i-1) );
}

#endif //__ARRAY_UNIT_TEST_IMPL_H__
