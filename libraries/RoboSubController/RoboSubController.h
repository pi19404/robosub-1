#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__

#include "RoboSubControllerData.h"
#include <Utility/NoCopy.h>
#include <Configurations/IMCS_Configuration.h>

#define PWM_MAX  127
#define PWM_MIN -127

#define N_CTRL_VALS 3
#define SEND_READY 'A'

class RoboSubController
    :
    public NoCopy
{    
    typedef int8_t CTRL_TYPE;

    public:
        RoboSubController(ComPort&, float);
        void Run();
        
    private:
        RoboSubControllerData            mState;
        Vector< CTRL_TYPE, N_CTRL_VALS > mLinearPWM;
        Vector< CTRL_TYPE, N_CTRL_VALS > mAngularPWM;
        ComPort                         &mComPort;
        unsigned long                    mLoopDelay;
};

RoboSubController::RoboSubController(ComPort &comPort, float loopFrequency)
    :
     mState()
    ,mLinearPWM()
    ,mAngularPWM()
    ,mComPort(comPort)
    ,mLoopDelay( (loopFrequency) ? (unsigned long)(1000/loopFrequency) : 0 )
{ }

void RoboSubController::Run()
{
    
    uint8_t sendData[5] = { 0, 0, 0, 0, 0 };
    int8_t recvData[5] = { 0, 0, 0, 0, 0  };
    int16_t pwmValues[6] = { 0, 0, 0, 0, 0, 0 };
    RoboSubControllerData newState;
    uint8_t fireStatus;
    LogManager& _lm = LogManager::GetInstance();
    unsigned long time, prev;
    
    while( true )
    {
        // Start time
        prev = millis();
        
        // Check if run switch disabled
        sendData[0] = ( _RUN ) ? (SEND_READY) : 0;
        
        // Read Compass Value (divide heading by 2)
        COMPASS.GetData();
        int16_t heading = COMPASS_DATA[CompassData<float>::Heading];
        sendData[1] = (uint8_t)(heading >>   8  ); // high byte
        sendData[2] = (uint8_t)(heading & 0x00FF); // low  byte
        
        // Read Depth
        DEPTH_DATA = int(analogRead(DEPTH_PIN) * DEPTH_SCALE);
        sendData[3] = ((uint8_t)((int16_t)DEPTH_DATA >> 8))&0x03; 
        sendData[4] =  (uint8_t)((int16_t)DEPTH_DATA & 0x00FF );
        
        // Send Data
        mComPort.write( sendData, sizeof(sendData)/sizeof(uint8_t) );
        
        // Get command data:
        mComPort.read( (uint8_t *)recvData, sizeof(recvData)/sizeof(int8_t) );
                
<<<<<<< local
        // Copy data
=======
        // Check _RUN switch is enabled
>>>>>>> other
        if( !_RUN )
<<<<<<< local
=======
        {
            // Command all thrusters to full stop
            for( int i = 0; i < (N_CTRL_VALS * 2); ++i )
            {
                THRUSTERS_DATA[i] = 0;
            }
        }
        // Update Control Data
        else
>>>>>>> other
        {
            // all stop
            for( int i = 0; i < (N_CTRL_VALS * 2); ++i )
            {
                THRUSTERS_DATA[i] = 0;
            }
        }
        else
        {
				// all go
            newState = RoboSubControllerData( recvData );
            
            // Check reset pin
            if( newState.Status & 0x80 )
            {
                // reset by returning
                return;
            }
            
            // Single-fire devices remain high after first fire command
            newState.Status = (((mState.Status | newState.Status)&0x0F) | (newState.Status & 0xF0));
            
            // Fire status is what is sent to the Pneumatics Plant
            // Fire commands after the first for single-fire devices are ignored
            fireStatus = ((newState.Status ^ mState.Status)&0x0F) | (newState.Status & 0xF0);
            
            // Update State
            mState = newState;
            
            // Update Pneumatics
            for( int i = 0; i < NUM_PNEUMATICS; ++i )
            {
                PNEUMATICS_DATA[i] = fireStatus & (1 << i);
            }
            
            // Get Thruster PWM Values from State
            mLinearPWM[0] = mState.OffsetX;
            mLinearPWM[1] = mState.OffsetY;
            mLinearPWM[2] = mState.OffsetZ;
            mAngularPWM[0] = 0;
            mAngularPWM[1] = mState.OffsetH;
            mAngularPWM[2] = 0;
            
<<<<<<< local
            // Compute Thruster Angular PWM Values:
            // mAngularError[0] = 0.0;//(CTRL_TYPE)mCompassData[CompassData<float>::Roll ]; we have no roll for strafing anymore.
            // mAngularError[1] = (CTRL_TYPE)((mState.OffsetHH << 8 ) + mState.OffsetHL );
            // mAngularError[2] = 0.0;//(CTRL_TYPE)mCompassData[CompassData<float>::Pitch ];
            
// begin update by John Cox 9-17-2012             
            // Update Thruster pwm values
            for( int i = 0, j = 0;  j < N_CTRL_VALS; i+=2, j+=1 )
            {
// ??why are thruster values created differently?  One is made with linear+angular, the other with linear-angular.  Won't they 
// always be different??

// ??pwm1 is created by an addition, so does it need a PWM_MIN check??
                int pwm1 = mLinearPWM[j] + mAngularPWM[j];
                if( pwm1 > PWM_MAX )
                {
                    pwm1 = PWM_MAX;
                }
                else if( pwm1 < PWM_MIN )
                {
                    pwm1 = PWM_MIN;
                }
                pwmValues[i]   = pwm1
                THRUSTERS_DATA[i] = pwmValues[i];
                _lm.Log( String( (int)THRUSTERS_DATA[i] ) );

// ??pwm2 is created by an addition, so does it need a PWM_MAX check??
                int pwm2 = mLinearPWM[j] - mAngularPWM[j];
                if( pwm2 > PWM_MAX )
                {
                    pwm2 = PWM_MAX;
                }
                else if( pwm2 < PWM_MIN )
                {
                    pwm2 = PWM_MIN;
                }
                pwmValues[i+1] = pwm2
                THRUSTERS_DATA[i+1] = pwmValues[i+1];
                _lm.Log( String( (int)THRUSTERS_DATA[i+1] ) );
            }

/*            
            // Scale Pwm Values
=======
            // Update Thruster PWM Values
>>>>>>> other
            for( int i = 0; i < (N_CTRL_VALS * 2); ++i )
            {
                // The first thruster of a pair is the sum of the PWMs
                if( (i % 2) == 0 )
                {
                    pwmValues[i] = mLinearPWM[i/2] + mAngularPWM[i/2];
                }
                // The second is the difference of the PWMs
                else
                {
                    pwmValues[i] = mLinearPWM[i/2] - mAngularPWM[i/2];
                }

                // Scale PWM Values:
                if( pwmValues[i] > PWM_MAX )
                {
                    pwmValues[i] = PWM_MAX;
                }
                else if( pwmValues[i] < PWM_MIN )
                {
                    pwmValues[i] = PWM_MIN;
                }

                // Update data of each thruster to scaled value
                THRUSTERS_DATA[i] = pwmValues[i];
            }
*/
// end update by John Cox 9-17-2012
            
            // mirror Y across X
            THRUSTERS_DATA[2] *= -1;
            THRUSTERS_DATA[3] *= -1;
        }
        
        // Send Pneumatics Commands
        PNEUMATIC_PLANT.CommandAll();
        
        // Send Thruster Commands
        THRUSTER_PLANT.DriveAll(); 
        
        // stop time; Check loop rate
        time = millis() - prev;
        if( time < mLoopDelay )
        {
            delay( mLoopDelay - time );
        }
    }
}

#endif //__ROBO_SUB_CONTROLLER_H__
