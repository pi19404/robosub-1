#ifndef __INPUT_PIPED_DEVICE_H__
#define __INPUT_PIPED_DEVICE_H__

#include "IPipeline.h"
#include "../Containers/Array.h"

// A device that receives input
// via pipelines
template <class T>
class IPipedDevice
{
    public:
    
        virtual void AttachInputLine( T& input )
        {
            mIPipes.PushBack( IPipeline<T>(&input) );
        }
        
    protected:
        Array< IPipeline<T> > mIPipes;
};

#endif //__INPUT_PIPED_DEVICE_H__
