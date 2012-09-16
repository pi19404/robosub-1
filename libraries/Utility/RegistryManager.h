#ifndef __REGISTRY_MANAGER_H__
#define __REGISTRY_MANAGER_H__

#include "Registry.h"

template <class T>
class RegistryManager
{
    public:
        static T* AddRegistryEntry( Registry<T>* registry, void* T )
        {
            if( registry )
            {
                return registry->AddEntry( entry );
            }
            return entry;
        }
};

#endif //__REGISTRY_MANAGER_H__
