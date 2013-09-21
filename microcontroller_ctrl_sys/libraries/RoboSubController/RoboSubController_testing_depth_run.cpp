#ifdef ROBOSUBCONTROLLER_TESTING_DEPTH_RUN

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
    mLogInstance.Log("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::Run()
{
    // for sensor readings
    uint16_t heading;
    uint16_t depthIn;
    uint16_t depthGoalFt = 5;
    uint16_t depthAllowableErrorIn = 10;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // initialize thruster data array to all nil values
    for (i = 0; i < NUM_THRUSTERS; i++)
    {
        pwmValues[i] = 0;
    }
    mCU.updateThrusters(pwmValues);

    // Read Depth
    // see comments in IMU function readDepth(...) for more detail
    depthIn = mIMU.readDepth();

    // for this test, only check the minimum boundary
    while( depthIn < ((depthGoalFt*12) - depthAllowableErrorIn))
    {
        // Start time
        prev = millis();

        mCU.updateThrusters(pwmValues);

        // stop time; Check loop rate
        time = millis() - prev;
        if( time < mLoopDelay )
        {
            delay( mLoopDelay - time );
        }

        // Read Depth
        // see comments in IMU function readDepth(...) for more detail
        depthIn = mIMU.readDepth();
    }


    // at depth;
    // shut off roll thrusters, set all drive thrusters forward for 10 seconds 
    // (arbitrarily chosen value), and do not attempt to maintain depth

    // Set Thruster PWM Values
    mLinearPWM[0] = 60;             // drive thrusters port
    mLinearPWM[1] = 60;             // drive thrusters starboard
    mLinearPWM[2] = 0;              // roll thrusters
    mAngularPWM[0] = 0;             // ??pitch or roll??
    mAngularPWM[1] = 0;             // desired heading
    mAngularPWM[2] = 0;             // ??pitch or roll??

    // Update Thruster PWM Values
    for( int i = 0; i < (NUM_THRUSTERS); ++i )
    {
        // The first thruster of a pair is the sum of the PWMs;
        // if there is to be a change in heading, one thruster will need 
        // to drive at a certain speed in order to attain that head, but
        // the other will drive more slowly so that the sub does not 
        // turn on a dime, so to speak, but rather turns like a moving 
        // vehicle making a corner
        if( (i % 2) == 0 )
        {
            // first thruster;
            // sum of PWMs
            pwmValues[i] = mLinearPWM[i/2] + mAngularPWM[i/2];
        }
        else
        {
            // second thruster;
            // sum of PWMs
            pwmValues[i] = mLinearPWM[i/2] - mAngularPWM[i/2];
        }

        // sanitize PWM Values so you don't burn out the thrusters
        if( pwmValues[i] > PWM_MAX )
        {
            pwmValues[i] = PWM_MAX;
        }
        else if( pwmValues[i] < PWM_MIN )
        {
            pwmValues[i] = PWM_MIN;
        }
    }

    mCU.updateThrusters(pwmValues);

    // delay 10 seconds;
    delay(10000);

    // end program by stopping thrusters and entering an infinite loop
    mCU.stopThrusters();
    while(1);
}


#endif  // ROBOSUBCONTROLLER_TESTING_THRUSTERS
