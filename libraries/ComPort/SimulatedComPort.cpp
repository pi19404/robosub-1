#ifdef BLUEBIRD

#include "SimulatedComPort.h"
#include "../Device/SimulatedDeviceController.h"
#include "../Device/SimulatedDevice.h"
#include <Logging/LogManager.h>


SimulatedComPort::SimulatedComPort(uint8_t device, SimulatedDeviceController& controller)
    :
ComPort(device)
    ,SimulatedDevice(controller)
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::SimulatedComPort(device, controller) constructor.");
}

SimulatedComPort::~SimulatedComPort() 
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::~SimulatedComPort() destructor.");
}

void SimulatedComPort::write(uint8_t* data, uint8_t nData)
{
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::write(data, nData).");

    read(data, nData, 0, 0);
}

void SimulatedComPort::read(uint8_t* data, uint8_t nData)
{
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::read(data, nData).");

    read(0, 0, data, nData );
}

void SimulatedComPort::read(uint8_t* wrdata, uint8_t nWrdata, uint8_t* rdata, uint8_t nRdata)
{
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::read(wrdata, nWrdata, rdata, nRdata).");

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
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::DataAvailable().");

    return !mSendBuffer.Empty(); 
}

uint8_t SimulatedComPort::ReadData()
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::ReadData().");

    return mSendBuffer.Dequeue(); 
}

void SimulatedComPort::WriteResponse( uint8_t data )
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("SimulatedComPort::WriteResponse(data).");

    mReceiveBuffer.Enqueue(data); 
}

#endif
