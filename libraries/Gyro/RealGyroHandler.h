#ifndef __REAL_GYRO_HANDLER_H__
#define __REAL_GYRO_HANDLER_H__

//Include Real Gyro Handler
#include "../MacroTricks/MacroTricks.h"
#if defined (GYRO)
    #include LOCAL_INCLUDE( JOIN(GYRO,_RealGyroHandler.h) )
#endif

#endif //__REAL_GYRO_HANDLER_H__
