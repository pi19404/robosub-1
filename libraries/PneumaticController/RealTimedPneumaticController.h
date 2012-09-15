#ifndef __REAL_TIMED_PNEUMATIC_CONTROLLER_H__
#define __REAL_TIMED_PNEUMATIC_CONTROLLER_H__

#include "RealPneumaticController.h"

class RealTimedPneumaticController
    :
    public RealPneumaticController
{
    public:
    
        using RealPneumaticController::AttachInputLine;
        
        RealTimedPneumaticController( int pin, 
                                      unsigned long interval )
            :
             RealPneumaticController(pin)
            ,mInterval(interval)
        {
        
        }
        
        using RealPneumaticController::Initialize;
        
        virtual void Drive()
        {
            if( mIPipes.Size() )
            {
                if( mIPipes[0].ReadFrom() )
                {
                    RealPneumaticController::Engage();
                    delay( mInterval );
                    RealPneumaticController::Release();
                }
            }   
        }
        
    private:
        unsigned long mInterval;
};

#endif //__REAL_TIMED_PNEUMATIC_CONTROLLER_H__
