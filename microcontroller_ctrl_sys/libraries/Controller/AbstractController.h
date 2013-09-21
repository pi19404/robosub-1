#ifndef __ABSTRACT_CONTROLLER_H__
#define __ABSTRACT_CONTROLLER_H__

#include "../Containers/Array.h"
#include "../Utility/NoCopy.h"

class AbstractController
    :
    public NoCopy
{
public:

    // Compute an output, u, based on an error e 
    virtual void Compute() = 0;
};

#endif //__ABSTRACT_CONTROLLER_H__
