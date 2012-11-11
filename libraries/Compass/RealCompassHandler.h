#ifndef __REAL_COMPASS_HANDLER_H__
#define __REAL_COMPASS_HANDLER_H__

#include <MacroTricks/MacroTricks.h>

//??verify: this adapted macro will make HMC6343/HMC6343_RealCompassHandler.h
#if defined COMP
    #include LOCAL_INCLUDE( JOIN( ( JOIN( COMP, / ) ), ( JOIN( COMP , _RealCompassHandler.h ) ) ) )
#endif

#endif //__REAL_COMPASS_HANDLER_H__
