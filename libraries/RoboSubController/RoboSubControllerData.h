#ifndef __ROBOSUB_CONTROLLER_DATA_H__
#define __ROBOSUB_CONTROLLER_DATA_H__

class RoboSubController;

class RoboSubControllerData
{
    friend class RoboSubController;
    
private:

    RoboSubControllerData()
        :
         Status()
        ,OffsetX()
        ,OffsetY()
        ,OffsetZ()
        ,OffsetH()
    { }
    
    RoboSubControllerData( const RoboSubControllerData& rhs )
        :
         Status(rhs.Status)
        ,OffsetX(rhs.OffsetX)
        ,OffsetY(rhs.OffsetY)
        ,OffsetZ(rhs.OffsetZ)
        ,OffsetH(rhs.OffsetH)
    { }
    
    explicit RoboSubControllerData( int8_t rhs[5] )
    {
         Status    = (uint8_t)rhs[0];
         OffsetX  = rhs[1];
         OffsetY  = rhs[2];
         OffsetZ  = rhs[3];
         OffsetH  = rhs[4];
    }
    
    // [0,7] = { Torpedo1, Torpedo2, MarkerDropper1, MarkerDropper2, Claw1Grip, Claw1Rot, Claw2Grip, Reset }
    uint8_t Status;
    
    int8_t   OffsetX  // X PWM
            ,OffsetY  // Y PWM
            ,OffsetZ  // Z PWM
            ,OffsetH // Heading PWM
            ;
};
    
#endif //__ROBOSUB_CONTROLLER_DATA_H__
