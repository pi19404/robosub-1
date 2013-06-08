#ifdef ROBOSUBCONTROLLER_TESTING_ACCEL

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort)
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // for the accelerometer data
    ACCEL_DATA myAccelData;

    // initialize thrusters to not moving
    mCU.stopThrusters();

    while( true )
    {
        mIMU.getAccelAll(&myAccelData);
        delay(500);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_ACCEL
