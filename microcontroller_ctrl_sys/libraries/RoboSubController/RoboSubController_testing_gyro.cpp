#ifdef ROBOSUBCONTROLLER_TESTING_GYRO

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

    // for the gyroscope data
    GYRO_DATA myGyroData;

    // initialize thrusters to not moving
    mCU.stopThrusters();

    while( true )
    {
        mIMU.getGyroAll(&myGyroData);
        delay(500);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_GYRO
