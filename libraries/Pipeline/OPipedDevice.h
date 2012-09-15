#ifndef __OUTPUT_PIPED_DEVICE_H__
#define __OUTPUT_PIPED_DEVICE_H__

#include "OPipeline.h"
#include "../Containers/Array.h"

// A device that transmits output
// via pipelines
template <class T>
class OPipedDevice
{
    public:
    
        void AttachOutputLine( T& output )
        {
            mOPipes.PushBack( OPipeline<T>(&output) );
        }
        
    protected:
        Array< OPipeline<T> > mOPipes;
};

#endif //__OUTPUT_PIPED_DEVICE_H__
