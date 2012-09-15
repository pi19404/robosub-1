#ifndef __DERIVATIVE_CONTROLLER_H__
#define __DERIVATIVE_CONTROLLER_H__

#include "TimeDependentController.h"
#include "../Pipeline/IOPipedDevice.h"

template <class T, class U>
class DerivativeController
    :
     public TimeDependentController
    ,public IOPipedDevice<T,U>
{
    using IOPipedDevice<T,U>::mIOPipes;
    
    public:
        DerivativeController( const T& kd ) 
            :
             TimeDependentController()
            ,IOPipedDevice<T,U>()
            ,mPrevError()
            ,mKd(kd)
            { }
        
        void  SetKd( T kd ){ mKd = kd; }
        T     GetKd( void ) const { return mKd; }
        
        virtual void AttachIOLine( T& source, U& dest )
        {
            // Attach line of data
            IOPipedDevice<T,U>::AttachIOLine( source, dest );
            
            // Add entry for previous error
            mPrevError.PushBack( T() );
        }
        
        // Compute an output, u, based on an error e 
        virtual void Compute( void )
        {   
            // Check if first computation:
            unsigned long curTime = millis();
            if( !TimeStamp() )
            {
                TimeStamp(curTime);
            }
            
            // Determine elapsed time, dt
            unsigned long dt = curTime - TimeStamp();
            TimeStamp(curTime);
            
            // Compute
            for( size_t i = 0; i < mIOPipes.Size(); ++i )
            {
                T Error = mIOPipes[i].ReadFrom();
                
                // Compute Derivative Term using numerical approximation of derivitive
                T dE = Error - mPrevError[i];
                mIOPipes[i].WriteTo( (dt > 0) ? dE*1000*mKd/dt: 0 );
            }
        }
        
        virtual void Reset()
        {
            for( int i = 0; i < mPrevError.Size(); ++i )
            {
                mPrevError[i] = 0;
            }
            
            TimeDependentController::Reset();
        }
        
    private:
        Array<T> mPrevError;
        T mKd;
};
        
#endif //__DERIVATIVE_CONTROLLER_H__
