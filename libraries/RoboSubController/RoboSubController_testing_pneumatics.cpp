#ifdef ROBOSUBCONTROLLER_TESTING_PNEUMATICS

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>
//#include <Arduino.h>

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
//    pDev myPneu;

//    myPneu.init(49);

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // pneumatic initialization done in CU constructor

    delay(1000);

//pinMode(52, OUTPUT);
    while( true )
    {
/*
        myPneu.activate();
        delay(4000);
        myPneu.deactivate();
        delay(4000);
*/
        mCU.fireTorpedoN(1);
        delay(2000);
        mCU.fireTorpedoN(2);
        delay(2000);
        mCU.dropMarkerN(1);
        delay(2000);
        mCU.dropMarkerN(2);
        delay(2000);


// all confirmed
// REMEMBER TO TURN ON POWER BEFORE ATTEMPTING!
// solenoid 1: arduino pin 52
// solenoid 2: arduino pin 53
// solenoid 3: arduino pin 49
// solenoid 4: arduino pin 43
// solenoid 5: arduino pin 41
// solenoid 6: arduino pin 31
//digitalWrite(52, HIGH);
//delay(3000);
//digitalWrite(52, LOW);
//delay(3000);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_PNEUMATICS

