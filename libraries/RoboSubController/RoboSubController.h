#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__

#include "./RoboSubControllerData.h"
#include "../Utility/NoCopy.h"
#include "../Configurations/IMCS_Configuration.h"

#include "../Controller/PDController.h"

#define PWM_MAX  127
#define PWM_MIN -127

#define LIN_KP (127.0L/1000.0L) //30.3398L
#define LIN_KD 21.942L
#define ANG_KP LIN_KP
#define ANG_KD LIN_KD
#define N_CTRL_VALS 3
typedef int8_t CTRL_TYPE;

#define SEND_READY 'A'

class RoboSubController
    :
    public NoCopy
{    

    public:
        RoboSubController(  
                            ComPort                                           &
                           ,float
                          );
                          
        void Run();
        
    private:
        RoboSubControllerData                              mState;
        //PDController< CTRL_TYPE, CTRL_TYPE >               mLinearController;
        //Vector< CTRL_TYPE, N_CTRL_VALS >                   mLinearError;
        Vector< CTRL_TYPE, N_CTRL_VALS >                   mLinearPWM;
        //PDController< CTRL_TYPE, CTRL_TYPE >               mAngularController;
        //Vector< CTRL_TYPE, N_CTRL_VALS >                   mAngularError;
        Vector< CTRL_TYPE, N_CTRL_VALS >                   mAngularPWM;
        
        ComPort                                           &mComPort;
        unsigned long                                      mLoopDelay;
};

RoboSubController::RoboSubController(  
                            ComPort                                           &comPort
                           ,float                                              loopFrequency
                          )
    :
     mState()
    //,mLinearController( LIN_KP, LIN_KD )
    //,mLinearError()
    ,mLinearPWM()
    //,mAngularController( ANG_KP, ANG_KD )
    //,mAngularError()
    ,mAngularPWM()
    ,mComPort(comPort)
    ,mLoopDelay( (loopFrequency) ? (unsigned long)(1000/loopFrequency) : 0 )
{ 

    // Setup Controllers:
    // for( int i = 0; i < N_CTRL_VALS; ++i )
    // {
        // mLinearController.AttachIOLine ( mLinearError[i], mLinearPWM[i] );
        // mAngularController.AttachIOLine(mAngularError[i],mAngularPWM[i] );
    // }
}

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
                
        // Copy data
        if( _RUN )
        {
            newState = RoboSubControllerData( recvData );
            
            // check reset
            if( newState.Status & 0x80 )
            {
                // call reset function:
                return;
            }
            
            // compute new status
            newState.Status = ( ( ( mState.Status | newState.Status )&0x0F ) | ( newState.Status & 0xF0 ) );
            
            // Update fire status
            fireStatus = ( ( newState.Status ^ mState.Status )&0x0F ) | ( newState.Status & 0xF0 );
            
            // update mState
            mState = newState;
            
            // Update Pneumatics
            for( int i = 0; i < NUM_PNEUMATICS; ++i )
            {
                PNEUMATICS_DATA[i] = fireStatus & (1 << i);
            }
            
            //Compute Thruster Linear PWM Values:
            // mLinearError[0] = (CTRL_TYPE)((mState.OffsetXH << 8 ) + mState.OffsetXL );
            // mLinearError[1] = (CTRL_TYPE)((mState.OffsetYH << 8 ) + mState.OffsetYL );
            // mLinearError[2] = (CTRL_TYPE)((mState.OffsetZH << 8 ) + mState.OffsetZL );
            mLinearPWM[0] = mState.OffsetX;
            mLinearPWM[1] = mState.OffsetY;
            mLinearPWM[2] = mState.OffsetZ;
            
            mAngularPWM[0] = 0;
            mAngularPWM[1] = mState.OffsetH;
            mAngularPWM[2] = 0;
            
            // Compute Thruster Angular PWM Values:
            // mAngularError[0] = 0.0;//(CTRL_TYPE)mCompassData[CompassData<float>::Roll ]; we have no roll for strafing anymore.
            // mAngularError[1] = (CTRL_TYPE)((mState.OffsetHH << 8 ) + mState.OffsetHL );
            // mAngularError[2] = 0.0;//(CTRL_TYPE)mCompassData[CompassData<float>::Pitch ];
            
                
            // Update Thruster pwm values
            for( int i = 0, j = 0;  j < N_CTRL_VALS; i+=2, j+=1 )
            {
                pwmValues[i]   = mLinearPWM[j] + mAngularPWM[j];
                pwmValues[i+1] = mLinearPWM[j] - mAngularPWM[j];
            }
            
            // Scale Pwm Values
            for( int i = 0; i < (N_CTRL_VALS * 2); ++i )
            {
                if( pwmValues[i] > PWM_MAX )
                {
                    pwmValues[i] = PWM_MAX;
                }
                else if( pwmValues[i] < PWM_MIN )
                {
                    pwmValues[i] = PWM_MIN;
                }
                
                THRUSTERS_DATA[i] = pwmValues[i];
                _lm.Log( String( (int)THRUSTERS_DATA[i] ) );
            }
            
            // mirror Y across X
            THRUSTERS_DATA[2] *= -1;
            THRUSTERS_DATA[3] *= -1;
        }
        else
        {
            // all stop
            for( int i = 0; i < (N_CTRL_VALS * 2); ++i )
            {
                THRUSTERS_DATA[i] = 0;
            }
        }
        
        // Run Pneumatics
        PNEUMATIC_PLANT.CommandAll();
        
        // Run Thrusters
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
