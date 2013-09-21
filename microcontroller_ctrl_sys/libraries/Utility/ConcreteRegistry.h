#ifndef __CONCRETE_REGISTRY_H__
#define __CONCRETE_REGISTRY_H__

#include "Singleton.h"
#include "Registry.h"

template <class T>
class ConcreteRegistry
    :
    public Singleton<T>
    ,public Registry<T>
{
public:
    // Delete all registered Devices
    ~ConcreteRegistry()
    {
        while( mRegistry.mSize() )
        {
            T* ptr = mRegistry.PopBack();
            if(ptr)
            {
                delete ptr;
            }
        }
    }

    virtual T* AddEntry( T* ptr ) 
    { 
        mRegistry.PushBack( (T*)ptr ); 
        return ptr;
    }

private:
    Array<T*> mRegistry;
};

#endif //__CONCRETE_REGISTRY_H__
