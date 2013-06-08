#ifdef ROBOSUBCONTROLLER_TESTING_COMPASS

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort),
    mLinearPWM(),
    mAngularPWM(),
    mLoopDelay( (loopFrequency) ? (unsigned long)(1000/loopFrequency) : 0 )
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::sayHello()
{
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("hi there!");
    delay(2000);
    mLogInstance.LogStr("har there!");
    delay(2000);
    mLogInstance.LogStr("ho there!");
    delay(2000);
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // for the accelerometer data
    COMPASS_DATA myCompassData;

    // initialize thrusters to not moving
    mCU.stopThrusters();

    while( true )
    {
        mIMU.getCompassAll(&myCompassData);
        delay(500);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_COMPASS
