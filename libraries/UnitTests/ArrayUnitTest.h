#ifndef __ARRAY_UNIT_TEST_H__
#define __ARRAY_UNIT_TEST_H__

#include "..\TestFramework\UnitTest.h"

class ArrayUnitTest
    :
    public UnitTest
{
    public:
        virtual void Setup();
        
        virtual void RunTestCases();
        
    // Test Cases:
    private:
        
        // Default Constructor
        void TestCaseDefaultConstructor();
        
        // Create Empty Array
        void TestCaseEmptyArray();
        
        // Copy Constructor
        void TestCaseCopyConstructor();
        
        // Self Assignment
        void TestCaseSelfAssignment();
        
        // Create Array too large
        void TestCaseArrayTooLarge();
        
        // Index outide of array bounds
        void TestCaseIndexOutsideBounds();
        
        // Index deleted element
        void TestCaseIndexDeletedElement();
};

#include "ArrayUnitTest_impl.h"

#endif //__ARRAY_UNIT_TEST_H__
