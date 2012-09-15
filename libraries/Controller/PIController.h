#ifndef __PI_CONTROLLER_H__
#define __PI_CONTROLLER_H__

#include "ProportionalController.h"
#include "IntegralController.h"
#include "../Pipeline/IOPipedDevice.h"

template <class T, class U>
class PIController
    :
     public TimeDependentController
    ,public IOPipedDevice<T,U>
{
    using IOPipedDevice<T,U>::mIOPipes;
    
    public:
        PIController( const T& kp, const T& ki )
            :
             TimeDependentController()
            ,IOPipedDevice<T,U>()
            ,mPController(kp)
            ,mIController(ki)
            ,mPControllerOutput()
            ,mIControllerOutput()
        { }

        virtual void AttachIOLine( T& source, U& dest )
        {
            // Attach PI Datalines
            IOPipedDevice<T,U>::AttachIOLine( source, dest );
            
            // Allocate Space on mPControllerOutput and mIControllerOutput
            size_t index = mPControllerOutput.PushBack( T() );
                           mIControllerOutput.PushBack( T() );
            
            // Attach P and I to the input, and their respective outputs:
            mPController.AttachIOLine( source, mPControllerOutput[index] );
            mIController.AttachIOLine( source, mIControllerOutput[index] );
        }

        virtual void Compute()
        {
            // Run P Controller
            mPController.Compute(); // Output is in mPControllerOutput;
            
            // Run I Controller
            mIController.Compute(); // Output is in mIControllerOutput;
            
            // Sum Outputs and send to output data line
            for( size_t i = 0; i < mIOPipes.Size(); ++i )
            {
                mIOPipes[i].WriteTo( mPControllerOutput[i] + mIControllerOutput[i] );
            }
        }     
    
        virtual void Reset(){ mIController.Reset(); }
        
    private:
        ProportionalController<T,T> mPController;
        IntegralController<T,T>     mIController;
        Array<T>                    mPControllerOutput;
        Array<T>                    mIControllerOutput;    
};


#endif //_PI_CONTROLLER_H__
