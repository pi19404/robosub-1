#ifndef __THRUSTER_H__
#define __THRUSTER_H__

#include <stdint.h>

class Thruster
{
public:
    virtual ~Thruster(){ }
    
    virtual void Drive() = 0;
};

#endif //__THRUSTER_H__