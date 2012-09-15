#ifndef __DEVICE_HANDLER_H__
#define __DEVICE_HANDLER_H__

#include "../Utility/NoCopy.h"

class ComPort;

class DeviceHandler
    :
    public NoCopy
{
public:
    virtual void Initialize() = 0;
};

#endif //__DEVICE_HANDLER_H__
