#ifdef ROBOSUBCONTROLLER_TESTING_PRESSURE

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

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i;

    // a local instance of the log manager
    LogManager& mLogInstance = LogManager::GetInstance();

    // for the depth sensor
    int16_t myDepth;

    // initialize thrusters to not moving
    mCU.stopThrusters();

    while( true )
    {
        mLogInstance.LogStr("reading depth");
        myDepth = mIMU.readDepth();
        delay(500);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_PRESSURE
