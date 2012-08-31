#ifndef __PIPELINE_H__
#define __PIPELINE_H__

// Defines a pipeline which
// links to data at some other
// location.
template <class T>
struct Pipeline
{
    Pipeline( T* d = 0 ) : mData(d) { }
    T* mData;
};

#endif //__PIPELINE_H__