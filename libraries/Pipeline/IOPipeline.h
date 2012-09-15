#ifndef __IO_PIPELINE_H__
#define __IO_PIPELINE_H__

#include "IPipeline.h"
#include "OPipeline.h"

// Defines a pipeline which
// links to data at some other
// location.
template <class T, class U>
class IOPipeline
    : 
     public IPipeline<T>
    ,public OPipeline<U>
{
    public:
        IOPipeline( T* i = 0, U* o = 0 ) : IPipeline<T>(i), OPipeline<U>(o) { }
        
        using IPipeline<T>::ReadFrom;
        using OPipeline<U>::WriteTo;
};

#endif //__IO_PIPELINE_H__
