#ifndef __THRUSTER_PLANT_H__
#define __THRUSTER_PLANT_H__

#include "Thruster.h"

#include <stdint.h>
#include <stddef.h>

class ThrusterPlant
{
    public:
        virtual ~ThrusterPlant() { }
        
        // -name AttachThruster
        // -desc attaches a thruster and it's input source
        // -desc to the thruster plant.
        // -args Thruster& thruster source
        virtual void AttachThruster( Thruster& ) = 0;
        
        // -name DriveThrusters
        // -desc drives all thrusters in the plant
        virtual void DriveAll() = 0;
};

#endif //__THRUSTER_PLANT_H__
