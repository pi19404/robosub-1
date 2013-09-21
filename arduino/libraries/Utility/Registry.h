#ifndef __REGISTRY_H__
#define __REGISTRY_H__

template <class T>
class Registry
{
 public:
     virtual T* AddEntry( T* entry ) = 0;
};

#endif //__REGISTRY_H__
