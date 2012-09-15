#ifndef __PD_CONTROLLER_H__
#define __PD_CONTROLLER_H__

#include "ProportionalController.h"
#include "DerivativeController.h"
#include "../Pipeline/IOPipedDevice.h"

template <class T, class U>
class PDController
    :
     public TimeDependentController
    ,public IOPipedDevice<T,U>
{
    using IOPipedDevice<T,U>::mIOPipes;
    
    public:
        PDController( const T& kp, const T& kd )
            :
             TimeDependentController()
            ,IOPipedDevice<T,U>()
            ,mPController(kp)
            ,mDController(kd)
            ,mPControllerOutput()
            ,mDControllerOutput()
        { }

        virtual void AttachIOLine( T& source, U& dest )
        {
            // Attach PD Datalines
            IOPipedDevice<T,U>::AttachIOLine( source, dest );
            
            // Allocate Space on mPControllerOutput and mDControllerOutput
            size_t index = mPControllerOutput.PushBack( T() );
            mPController.AttachIOLine( source, mPControllerOutput[index] );
                   index = mDControllerOutput.PushBack( T() );
            mDController.AttachIOLine( source, mDControllerOutput[index] );
        }

        virtual void Compute()
        {
            // Run P Controller
            mPController.Compute(); // Output is in mPControllerOutput;
            
            // Run I Controller
            mDController.Compute(); // Output is in mDControllerOutput;
            
            // Sum Outputs and send to output data line
            for( size_t i = 0; i < mIOPipes.Size(); ++i )
            {
                mIOPipes[i].WriteTo( mPControllerOutput[i] + mDControllerOutput[i] );
            }
        }     
    
        virtual void Reset(){ mDController.Reset(); }
        
    private:
        ProportionalController<T,T> mPController;
        DerivativeController<T,T>   mDController;
        Array<T>                    mPControllerOutput;
        Array<T>                    mDControllerOutput;    
};


#endif //_PD_CONTROLLER_H__
