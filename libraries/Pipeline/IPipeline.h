#ifndef __INPUT_PIPELINE_H__
#define __INPUT_PIPELINE_H__

#include "Pipeline.h"

template <class T>
class IPipeline
    :
    protected Pipeline<T>
{
    public:
        IPipeline( T* t = 0 ) : Pipeline<T>(t) { }
        
        virtual const T& ReadFrom()
        {
            return *Pipeline<T>::mData;
        }
};

#endif //__INPUT_PIPELINE_H__
