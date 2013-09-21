#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include "ExplicitLifetime.h"

template <class T>
class Singleton
    :
    public ExplicitLifetime
{
public:
    ~Singleton()
    {
        if( mInstance )
        {
            delete mInstance;
        }
    }

    static T& GetInstance()
    { 
        if( !mInstance )
        {
            mInstance = new T;
        }
        return *mInstance;
    }

protected:
    Singleton() : ExplicitLifetime() { }

private:
    Singleton(const Singleton&);
    const Singleton<T>& operator=(const Singleton<T>& );

private:
    static T* mInstance;
};

template <class T>
T* Singleton<T>::mInstance = 0;

#endif //__SINGLETON_H__
