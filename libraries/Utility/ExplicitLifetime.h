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

    private:
        ExplicitLifetime(const ExplicitLifetime& );
        const ExplicitLifetime& operator=(const ExplicitLifetime&);
};
        
#endif //__EXPLICIT_LIFETIME_H__
