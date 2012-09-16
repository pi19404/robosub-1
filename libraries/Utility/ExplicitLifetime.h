#ifndef __EXPLICIT_LIFETIME_H__
#define __EXPLICIT_LIFETIME_H__

#include "NoCopy.h"

// A Class with an explicit lifetime
class ExplicitLifetime
    :
    public NoCopy
{
    public:
        virtual ~ExplicitLifetime() { }
        
    protected:
        ExplicitLifetime() : NoCopy() { }
};
        
#endif //__EXPLICIT_LIFETIME_H__
