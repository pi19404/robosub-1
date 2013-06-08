#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__

//#include "RoboSubControllerData.h"
#include <Utility/NoCopy.h>
#include <Configurations/DeviceConfig.h>
#include <Overseers/CU.h>
#include <Overseers/IMU.h>
#include <I2CDevices/CLS.h>

// ripped from IMCS_Configuration.h (??necessary??)
// Begin Interrupt
#define RUN_PIN 2
#define RUN_INT 0

// ??what is this??
#define SEND_READY 'A'

class RoboSubController
    :
    public NoCopy
{    

public:
    RoboSubController( ComPort&, float );

    void Run();

private:
    bool m_RUN;

    // this com port is for communicating with the AI
    ComPort &mComPort;

    // the control unit (for "action hardware (thrusters/pneumatics))
    CU mCU;

    // the inertial measurement unit (for sensors)
    IMU mIMU;

    // this is for a loop delay to keep the Run method's while(1) loop going at
    // a specified frequency
    unsigned long  mLoopDelay;
};

#endif //__ROBO_SUB_CONTROLLER_H__
