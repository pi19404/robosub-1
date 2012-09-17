#include "SimulatedComPort.h"
#include "../Device/SimulatedDeviceController.h"
#include "../Device/SimulatedDevice.h"

SimulatedComPort::SimulatedComPort( uint8_t device, SimulatedDeviceController& controller )
    :
     ComPort(device)
    ,SimulatedDevice(controller)
{ 

}

SimulatedComPort::~SimulatedComPort() 
{ 

}

void SimulatedComPort::write(uint8_t* data, uint8_t nData )
{
    read(data, nData, 0, 0);
}

void  SimulatedComPort::read (uint8_t* data, uint8_t nData)
{
    read(0, 0, data, nData );
}

void SimulatedComPort::read(uint8_t* wrdata, uint8_t nWrdata, uint8_t* rdata, uint8_t nRdata )
{
    // Send arguments
    for( uint8_t i = 0; i < nWrdata; ++i )
    {
        mSendBuffer.Enqueue( wrdata[i] );
    }
    
    // notify data ready
    NotifyController();
    
    // clock out received data
    for( uint8_t i = 0; i < nRdata; ++i )
    {
        rdata[i] = mReceiveBuffer.Dequeue();
    }
}

bool SimulatedComPort::DataAvailable()
{ 
    return !mSendBuffer.Empty(); 
}

uint8_t SimulatedComPort::ReadData()
{ 
    return mSendBuffer.Dequeue(); 
}

void SimulatedComPort::WriteResponse( uint8_t data )
{ 
    mReceiveBuffer.Enqueue(data); 
}
