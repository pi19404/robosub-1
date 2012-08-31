#ifndef __REAL_PNEUMATIC_PLANT_H__
#define __REAL_PNEUMATIC_PLANT_H__

#include "PneumaticPlant.h"
#include "..\Containers\Array.h"

class RealPneumaticPlant
    :
    public PneumaticPlant
{
    public:
        virtual ~RealPneumaticPlant() { }
        
        virtual void AttachPneumaticController( PneumaticController& controller )
        {
            mControllers.PushBack( &controller );
        }
        
        virtual void CommandAll()
        {
            for( size_t i = 0; i < mControllers.Size(); ++i )
            {
                mControllers[i]->Drive();
            }
        }
        
    private:
        Array< PneumaticController* > mControllers;
};

#endif //__REAL_PNEUMATIC_PLANT_H__