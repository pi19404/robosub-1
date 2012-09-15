#ifndef __REAL_THRUSTER_PLANT_H__
#define __REAL_THRUSTER_PLANT_H__

#include "Thruster.h"
#include "ThrusterPlant.h"
#include "../Containers/Array.h"

class RealThrusterPlant
    :
     public ThrusterPlant
{
    public:
        RealThrusterPlant()
            :
             ThrusterPlant()
            ,mThrusters()
        { }
        
        // -name AttachThruster
        // -desc attaches a thruster and it's input source
        // -desc to the thruster plant.
        // -args Thruster& thruster
        virtual void AttachThruster( Thruster& thruster )
        {
              mThrusters.PushBack( &thruster );
        }
        
        // -name DriveThrusters
        // -desc drives all thrusters in the plant
        virtual void DriveAll()
        {
            for( size_t i = 0; i < mThrusters.Size(); ++i )
            {
                mThrusters[i]->Drive();
            }
        }
    
    private:
        Array< Thruster* > mThrusters;
};

#endif //__REAL_THRUSTER_PLANT_H__
