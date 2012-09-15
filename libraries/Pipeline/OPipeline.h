#ifndef __OUTPUT_PIPELINE_H__
#define __OUTPUT_PIPELINE_H__

#include "Pipeline.h"
template <class T>
class OPipeline
    :
    protected Pipeline<T>
{
    public:
    
        explicit OPipeline( T* t = 0 ) : Pipeline<T>(t) { }
 
        virtual void WriteTo(const T& data )
        {
            *Pipeline<T>::mData = data;
        }
};

#endif //__OUTPUT_PIPELINE_H__
