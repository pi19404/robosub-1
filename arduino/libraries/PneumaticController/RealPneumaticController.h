#ifndef __REAL_PNEUMATIC_CONTROLLER_H__
#define __REAL_PNEUMATIC_CONTROLLER_H__

#include "PneumaticController.h"

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include "../Pipeline/IPipedDevice.h"
#include "PneumaticControllerData.h"

class RealPneumaticController
    :
     public PneumaticController
    ,public IPipedDevice< PneumaticControllerData >
{
    public:
        
        RealPneumaticController( int pin )
            :
             PneumaticController()
            ,mPin(pin)
        {
                      
        }
        
        virtual void AttachInputLine( PneumaticControllerData& data )
        {
            // only allow one connection
            if( ! mIPipes.Size() )
            {
                IPipedDevice<PneumaticControllerData>::AttachInputLine( data );
            }
            else
            {
                mIPipes[0] = &data;
            }
        }
        
        virtual void Initialize()
        {
            pinMode( mPin, OUTPUT );
            Release();
        }
        
        virtual void Drive()
        {
            if( mIPipes.Size() )
            {
                if( mIPipes[0].ReadFrom() )
                {
                    Engage();
                }
                else
                {
                    Release();
                }
            }
        }
       
    protected:
        void Engage()
        {
            digitalWrite( mPin, HIGH );
        }
        
        void Release()
        {
            digitalWrite( mPin, LOW );
        }
    
    protected:
        using IPipedDevice< PneumaticControllerData >::mIPipes;
        
    private:
        int mPin;
};

#endif //__REAL_PNEUMATIC_CONTROLLER_H__
