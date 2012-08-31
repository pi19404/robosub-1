#ifndef __DEVICE_REGISTRY_H__
#define __DEVICE_REGISTRY_H__

#include "..\Utility\ConcreteRegistry.h"
#include "DeviceHandler.h"

typedef ConcreteRegistry<DeviceHandler> DeviceRegistry;

#endif //__DEVICE_REGISTRY_H__