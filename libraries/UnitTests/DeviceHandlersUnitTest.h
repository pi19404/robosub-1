#ifndef __DEVICES_HANDLERS_UNIT_TEST_H__
#define __DEVICES_HANDLERS_UNIT_TEST_H__

#include "RealCompassHandlerUnitTest.h"
#include "RealMotorCtrlHandlerUnitTest.h"
#include "RealGyroHandlerUnitTest.h"

RealCompassHandlerUnitTest   _CompassTest;
RealMotorCtrlHandlerUnitTest _MotorCtrlTest;
RealGyroHandlerUnitTest      _GyroTest;

UnitTest* const Tests[] = {
                    &_CompassTest
                   ,&_MotorCtrlTest
                   ,&_GyroTest
};
const size_t NUM_TESTS = (sizeof(Tests)/sizeof(UnitTest*));

class DeviceHandlersUnitTest
    :
    public UnitTest
{
    public:
        
        virtual void Setup()
        {
            for( size_t i = 0; i < NUM_TESTS; ++i )
            {
                Tests[i]->Setup();
            }
        }
        
        virtual void RunTestCases()
        {
            for( size_t i = 0; i < NUM_TESTS; ++i )
            {
                Tests[i]->RunTestCases();
            }
        }
};

#endif //__DEVICES_HANDLERS_UNIT_TEST_H__