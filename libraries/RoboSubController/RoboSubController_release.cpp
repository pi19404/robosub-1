#ifdef ROBOSUBCONTROLLER_RELEASE

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
    mLogInstance.Log("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::sayHello()
{
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.Log("hi there!");
    delay(2000);
    mLogInstance.Log("har there!");
    delay(2000);
    mLogInstance.Log("ho there!");
    delay(2000);
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i;

    // buffers for serial transmission and reception with PC
    uint8_t sendData[5] = { 0, 0, 0, 0, 0 };
    int8_t recvData[5] = { 0, 0, 0, 0, 0  };

    // temporary storage for thruster pwm calculations based on AI commands
    int16_t pwmValues[NUM_THRUSTERS];

    // better named storage for the five bytes of the serial receive data, which
    // carries commands from the AI
    uint8_t statusReg;
    uint8_t thrusterCmdX;
    uint8_t thrusterCmdY;
    uint8_t thrusterCmdZ;
    uint8_t headingCmd;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    // for the loop delay
    unsigned long time;
    unsigned int prev;

    // for sensor readings
    int16_t heading;
    int depth;

    // initialize thruster data array to all nil values
    for (i = 0; i < NUM_THRUSTERS; i++)
    {
        pwmValues[i] = 0;
    }

    while( true )
    {
        // Start time
        prev = millis();

        // Check if run switch disabled
        if( digitalRead(RUN_PIN) )
        {
            m_RUN = true;
        }
        else
        {
            m_RUN = false;
        }

        // begin filling out the data array to send to the PC;
        // first, check if the run pin is enabled
        sendData[0] = ( m_RUN ) ? (SEND_READY) : 0;

        // Note: sensor data is 16 bit integer, but need to split it into 1 
        // byte chunks for serial transmission

        // Read Compass value;
        mIMU.readCompass();
        heading = mIMU.getCompassHeading();
        sendData[1] = (uint8_t)(heading >> 8);      // high byte
        sendData[2] = (uint8_t)(heading & 0x00FF);  // low  byte

        // Read Depth
        depth = mIMU.readDepth();
        sendData[3] = ((uint8_t)((int16_t)depth >> 8)) & 0x03; 
        sendData[4] =  (uint8_t)((int16_t)depth & 0x00FF );


// these are for PC (read, "AI") communication; disable for testing
        // Send Data to PC
        //mComPort.write( sendData, sizeof(sendData)/sizeof(uint8_t) );

        // Get command data from PC (AI or test setup)
        //mComPort.read( (uint8_t *)recvData, sizeof(recvData)/sizeof(int8_t) );


        statusReg = recvData[0];
        thrusterCmdX = recvData[1];
        thrusterCmdY = recvData[2];
        thrusterCmdZ = recvData[3];
        headingCmd = recvData[4];

        // Check m_RUN switch (is interrupt driven)
        if( !m_RUN )
        {
            // Command all thrusters to full stop
            mCU.stopThrusters();
        }
        else
        {
            // Update Control Data

            // Status byte:
            // bit 0 - Torpedo1
            // bit 1 - Torpedo2
            // bit 2 - MarkerDropper1
            // bit 3 - MarkerDropper2
            // bit 4 - Claw1Grip
            // bit 5 - Claw1Rot
            // bit 6 - Claw2Grip
            // bit 7 - Reset (highest priority)
            if (statusReg & 0x80)
            {
                // AI says to reset, so return to the Arduino loop
                return;
            }
            if (statusReg & 0x40)
            {
                // claw 2 grip
            }
            if (statusReg & 0x20)
            {
                // claw 1 rotation
            }
            if (statusReg & 0x10)
            {
                // claw 1 grip
            }
            if (statusReg & 0x08)
            {
                // request to drop marker 2
                mCU.dropMarkerN(2);
            }
            if (statusReg & 0x04)
            {
                // request to drop marker 1
                mCU.dropMarkerN(1);
            }
            if (statusReg & 0x02)
            {
                // request to fire torpedo 2
                mCU.fireTorpedoN(2);
            }
            if (statusReg & 0x01)
            {
                // request to fire torpedo 1
                mCU.fireTorpedoN(1);
            }

            // Get Thruster PWM Values
            mLinearPWM[0] = thrusterCmdX;   // left/right speed
            mLinearPWM[1] = thrusterCmdY;   // forward/backward speed
            mLinearPWM[2] = thrusterCmdZ;   // up/down speed
            mAngularPWM[0] = 0;             // ??pitch or roll??
            mAngularPWM[1] = headingCmd;    // desired heading
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

                if (2 == i || 3 == i)
                {
                    // this is a "band aid" fix for now;
                    // for some reason, the forward thrusters are turning the 
                    // wrong directions;
                    // reverse them;
                    pwmValues[i] *= -1;
                }
            }

            mCU.updateThrusters(pwmValues);
        }

        // stop time; Check loop rate
        time = millis() - prev;
        if( time < mLoopDelay )
        {
            delay( mLoopDelay - time );
        }
    }
}

#endif  // ROBOSUBCONTROLLER_RELEASE
