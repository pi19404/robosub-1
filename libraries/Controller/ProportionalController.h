#ifndef __PROPORTIONAL_CONTROLLER_H__
#define __PROPORTIONAL_CONTROLLER_H__

#include "AbstractController.h"
#include "../Pipeline/IOPipedDevice.h"

template <class T, class U>
class ProportionalController
    :
     public AbstractController
    ,public IOPipedDevice<T,U>
{   
    using IOPipedDevice<T,U>::mIOPipes;
    
    public:
        ProportionalController( const T& kp ) 
            :
             AbstractController()
            ,IOPipedDevice<T,U>()
            ,mKp(kp)
            { }
        
        void  SetKp( T kp ){ mKp = kp; }
        T GetKp( void ) const { return mKp; }
        
        virtual void AttachIOLine( T& source, U& dest )
        {
            // Attach line of data
            IOPipedDevice<T,U>::AttachIOLine( source, dest );
        }
        
        // Compute an output, u, based on an error e 
        virtual void Compute( void )
        {   
            // Compute
            for( size_t i = 0; i < mIOPipes.Size(); ++i )
            {
                // Compute Proportional Term using numerical approximation of proportional
                U target = mIOPipes[i].ReadFrom()*mKp;
                mIOPipes[i].WriteTo( target );
            }
        }
        
    private:
        T mKp;
};
        
#endif //__PROPORTIONAL_CONTROLLER_H__
