#ifndef __SIMULATED_DEVICE_CONTROLLER_H__
#define __SIMULATED_DEVICE_CONTROLLER_H__

class SimulatedDevice;

class SimulatedDeviceController
{
    public:
        virtual void DataAvailable(SimulatedDevice &) = 0;
};

#endif //__SIMULATED_DEVICE_CONTROLLER_H__
