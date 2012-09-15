#ifndef __PNEUMATIC_CONTROLLER_H__
#define __PNEUMATIC_CONTROLLER_H__

#include "../Device/DeviceHandler.h"

class PneumaticController
    :
    public DeviceHandler
{
    public:
        virtual void Initialize() = 0;
        virtual void Drive() = 0;
};

#endif //__PNEUMATIC_CONTROLLER_H__
