#ifndef __REAL_THRUSTER_H__
#define __REAL_THRUSTER_H__

#include "Thruster.h"
#include "ThrusterData.h"
#include "../MotorCtrl/MotorCtrlHandler.h"
#include "../Pipeline/IPipedDevice.h"

class RealThruster
    :
     public Thruster
    ,public IPipedDevice< ThrusterData >
{   
    public:
        RealThruster(  MotorCtrlHandler& controller
                      ,Motor::Enum motor )
            :
             Thruster()
            ,mController( controller )
            ,mMotor( motor )
        { }
        
        virtual void AttachInputLine( ThrusterData& input )
        {
            // Only allow one input line
            if ( ! mIPipes.Size() )
            {
                IPipedDevice< ThrusterData >::AttachInputLine( input );
            }
            else
            {
                mIPipes[0] = &input;
            }
        }
    
        virtual void Drive()
        {
            if( mIPipes.Size() )
            {
                mController.DriveMotor( mMotor, mIPipes[0].ReadFrom() );
            }
        }
        
    private:
        using IPipedDevice< ThrusterData >::mIPipes;
        MotorCtrlHandler& mController;
        Motor::Enum       mMotor;
};
    
    
    

#endif //__REAL_THRUSTER_H__
