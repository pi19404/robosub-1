#ifndef __RCHUT_H__
#define __RCHUT_H__

#define COMPASS HMC6343

#include "../Containers/Queue.h"
#include "../ComPort/SimulatedComPort.h"

#include "../TestFramework/UnitTest.h"
#include "../Device/SimulatedDeviceController.h"

#include "../Compass/RealCompassHandler.h"

#include <stdint.h>

class RealCompassHandlerUnitTest
    :
     public UnitTest
    ,public SimulatedDeviceController
{
    public:
        RealCompassHandlerUnitTest();
        
        virtual void Setup();
        
        virtual void RunTestCases();
        
    private:
        // Regular Commands:
        void TestCommands();
        
        // Eeprom Commands
        void TestDeviceConfig();
        void TestMeasurementRate();
        
    private:
        // Helper Methods
        virtual void DataAvailable( SimulatedDevice& );
        
    private:
        Queue<uint8_t>          mReceiveBuffer;
        SimulatedComPort        mComPort;
        RealCompassHandler      mCompass;
        CompassData<float>      mCompassData;
};

#include "RealCompassHandlerUnitTest_impl.h"

#endif //__RCHUT_H__
