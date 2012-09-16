#ifndef __SIMULATED_DEVICE_H__
#define __SIMULATED_DEVICE_H__

#include "SimulatedDeviceController.h"

#include <stdint.h>

class SimulatedDevice
{
    public:
        SimulatedDevice( SimulatedDeviceController& controller )
            :
            mController(controller)
        { }
        
        virtual ~SimulatedDevice() { }
        
        /*
         * Check if data is still available
         */
        virtual bool DataAvailable() = 0;
      
        /*
         *  Read data from the port
         */
        virtual uint8_t ReadData() = 0;
        
        /*
         *  Write data to the port
         */
        virtual void WriteResponse( uint8_t ) = 0;
        
    protected:
    
        void NotifyController()
        {
            mController.DataAvailable(*this);
        }
        
    private:
        SimulatedDeviceController& mController;
};

#endif //__SIMULATED_DEVICE_H__
