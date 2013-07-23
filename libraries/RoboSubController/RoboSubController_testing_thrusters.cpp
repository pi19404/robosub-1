#ifdef ROBOSUBCONTROLLER_TESTING_THRUSTERS

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort),
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

    // temporary storage for thruster pwm calculations based on AI commands
    int dutyCycles[NUM_THRUSTERS];
    int motorDirections[NUM_THRUSTERS];

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // pause for a moment before beginning
    delay(2000);

    // default all thrusters to forward (motor direction = 0) according to the
    // commands required by the h bridge
    for(i = 0; i < NUM_THRUSTERS; i++)
    {
        dutyCycles[i] = 0;
        motorDirections[i] = 0;
    }
/*
    // set all drive thrusters to drive forward at 50% duty cycle
    for (i = 0; i < 4; i++)
    {
        dutyCycles[i] = 20;
        motorDirections[i] = 1;
    }
    
    // start the roll thrusters off
    dutyCycles[4] = 0;
    dutyCycles[5] = 0;
    motorDirections[4] = 1;
    motorDirections[5] = 1;

    _lm.LogStr("starting up drive thrusters at 50% and roll at 0");
    mCU.setThrusters(dutyCycles, motorDirections);
*/

    while( true )
    {
        // periodically turn the thrusters on and off
/*
        _lm.LogStr("turning port thrusters on");
        dutyCycles[0] = 30;
        dutyCycles[1] = 30;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(2000);

        _lm.LogStr("turning port thrusters off");
        dutyCycles[0] = 0;
        dutyCycles[1] = 0;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(4000);

        _lm.LogStr("turning starboard thrusters on");
        dutyCycles[2] = 20;
        dutyCycles[3] = 20;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(2000);

        _lm.LogStr("turning starboard thrusters off");
        dutyCycles[2] = 0;
        dutyCycles[3] = 0;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(4000);

        _lm.LogStr("turning roll thrusters on");
        dutyCycles[4] = 30;
        dutyCycles[5] = 30;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(2000);

        _lm.LogStr("turning roll thrusters off");
        dutyCycles[4] = 0;
        dutyCycles[5] = 0;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(4000);
*/
        _lm.LogStr("turning thrusters on");
        dutyCycles[0] = 40;
        dutyCycles[1] = 40;
        dutyCycles[2] = 40;
        dutyCycles[3] = 40;
        dutyCycles[4] = 40;
        dutyCycles[5] = 40;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(2000);

        _lm.LogStr("turning thrusters off");
        dutyCycles[0] = 0;
        dutyCycles[1] = 0;
        dutyCycles[2] = 0;
        dutyCycles[3] = 0;
        dutyCycles[4] = 0;
        dutyCycles[5] = 0;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(4000);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_THRUSTERS
