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

    while( true )
    {
        _lm.LogStr("turning roll thrusters on");

        // periodically turn the roll thrusters on and off
        dutyCycles[4] = 20;
        dutyCycles[5] = 20;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(2000);

        _lm.LogStr("turning roll thrusters off");
        dutyCycles[4] = 0;
        dutyCycles[5] = 0;
        mCU.setThrusters(dutyCycles, motorDirections);
        delay(5000);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_THRUSTERS
