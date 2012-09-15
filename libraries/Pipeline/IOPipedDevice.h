#ifndef __IO_PIPED_DEVICE_H__
#define __IO_PIPED_DEVICE_H__

#include "IOPipeline.h"
#include "../Containers/Array.h"

// A device that receives and
// transmits data via pipelines
template <class T, class U>
class IOPipedDevice
{
    public:
    
        virtual void AttachIOLine( T& input, U& output )
        {
            mIOPipes.PushBack( IOPipeline<T,U>( &input, &output ) );
        }
        
    protected:
        Array< IOPipeline<T,U> > mIOPipes;
};

#endif //__IO_PIPED_DEVICE_H__
