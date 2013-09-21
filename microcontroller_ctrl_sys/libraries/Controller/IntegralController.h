#ifndef __INTEGRAL_CONTROLLER_H__
#define __INTEGRAL_CONTROLLER_H__

#include "TimeDependentController.h"
#include "../Pipeline/IOPipedDevice.h"

template <class T, class U>
class IntegralController
    :
    public TimeDependentController
    ,public IOPipedDevice<T,U>
{
    using IOPipedDevice<T,U>::mIOPipes;

public:
    IntegralController( const T& kd ) 
        :
    TimeDependentController()
        ,IOPipedDevice<T,U>()
        ,mKi(kd)
    { }

    void  SetKi( T kd ){ mKi = kd; }
    T     GetKi( void ) const { return mKi; }

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
        unsigned long dt = curTime - mTimeStamp;
        TimeStamp(curTime);

        // Compute
        for( size_t i = 0; i < mIOPipes.Size(); ++i )
        {
            T Error = mIOPipes[i].ReadFrom();

            // Compute Integral Term using numerical approximation of integral
            mIOPipes[i].WriteTo( Error*mKi*1000*T(dt) );
        }
    }

    using TimeDependentController::Reset;

private:
    T mKi;
};

#endif //__INTEGRAL_CONTROLLER_H__
