#ifdef ROBOSUBCONTROLLER_TESTING_CLS_GYRO

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort),
    mCU(),
    mIMU(),
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
    int i = 0;
    uint16_t myUShort = 0x7f38;
    int16_t myShort = 0x7f38;
    float ginger;

    char buffer[20];

    // this is the CLS pmod that stuff can be printed to
    CLS mLittleScreen;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // for the gyroscope data
    GYRO_DATA myGyroData;

    // initialize thrusters to not moving
    mCU.stopThrusters();

    while( true )
    {
/*
X = 'ffffed01'
X = 'c22a347b'
X = -42.55
Y = '2b00'
Y = '42c0a3d7'
Y = 96.32
Z = '47fd'
Z = '432140f6'
Z = 161.25

        ginger = -42.55;
        snprintf(buffer, 20, "X = '%x'", ginger);
        Serial.print(buffer);
        delay(1000);
*/
        mIMU.getGyroAll(&myGyroData);
        delay(3000);
//        ftoa();
//        sprintf(buffer, "i = '%d'", i);
//        mLittleScreen.writeStrToCLS(buffer);
//        i++;
//        delay(5000);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_CLS_GYRO
