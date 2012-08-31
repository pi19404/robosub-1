#ifndef __PNEUMATIC_PLANT_H__
#define __PNEUMATIC_PLANT_H__

#include "PneumaticController.h"

class PneumaticPlant
{
    public:
        virtual ~PneumaticPlant() { }
        
        virtual void AttachPneumaticController( PneumaticController& ) = 0;
        
        virtual void CommandAll() = 0;
};

#endif //__PNEUMATIC_PLANT_H__