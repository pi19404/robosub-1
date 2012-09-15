#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__

#include "../Math/Vector.h"
#include "../Thruster/ThrusterPlant.h"
#include "../Thruster/ThrusterData.h"
#include "../PneumaticController/PneumaticPlant.h"
#include "../PneumaticController/PneumaticControllerData.h"
#include "../Compass/CompassData.h"
#include "../Compass/CompassHandler.h"
#include "../Device/AnalogDevice.h"
#include "../ComPort/ComPort.h"
#include "../Utility/NoCopy.h"

class RoboSubController
    :
    public NoCopy
{    
    struct RoboSubControllerData
    {
        RoboSubControllerData()
            :
             Status()
            ,NunchuckX()
            ,NunchuckY()
        { }
        
        RoboSubControllerData( const RoboSubControllerData& rhs )
            :
             Status(rhs.Status)
            ,NunchuckX(rhs.NunchuckX)
            ,NunchuckY(rhs.NunchuckY)
        { }
        
        uint8_t Status;     // [0,7] = { Torpedo1, Torpedo2, MarkerDropper, ClawPos, ClawGrip, WiiUp, WiiDn, Reset }
         int8_t NunchuckX;
         int8_t NunchuckY;
    };

    public:
        RoboSubController(  
                            ComPort                              &
                           ,Vector< ThrusterData, 4 >            &
                           ,Vector< PneumaticControllerData, 5 > &
                           ,ThrusterPlant                        &
                           ,PneumaticPlant                       &
                           ,CompassData<float>                   &
                           ,CompassHandler<float>                &
                           ,int                                  
                           ,AnalogDevice                         &
                           ,float
                          );
                          
        void Run();
        
    private:
        RoboSubControllerData                 mState;
        ComPort                              &mComPort;
        Vector< ThrusterData, 4 >            &mThrusterData;
        Vector< PneumaticControllerData, 5 > &mPneumaticData;
        ThrusterPlant                        &mThrusterPlant;
        PneumaticPlant                       &mPneumaticPlant;
        CompassData<float>                   &mCompassData;
        CompassHandler<float>                &mCompass;
        int                                   mLeakPin;
        AnalogDevice                         &mBattVoltage;
        unsigned long                         mLoopDelay;
};

RoboSubController::RoboSubController(  
                            ComPort                              &comPort
                           ,Vector< ThrusterData, 4 >            &thrusterData   
                           ,Vector< PneumaticControllerData, 5 > &pneumaticData
                           ,ThrusterPlant                        &thrusterPlant  
                           ,PneumaticPlant                       &pneumaticPlant
                           ,CompassData<float>                   &compassData
                           ,CompassHandler<float>                &compass
                           ,int                                   leakPin
                           ,AnalogDevice                         &battVoltage
                           ,float                                loopFrequency
                          )
    :
     mState()
    ,mComPort(comPort)
    ,mThrusterData(thrusterData)
    ,mPneumaticData(pneumaticData)
    ,mThrusterPlant(thrusterPlant)
    ,mPneumaticPlant(pneumaticPlant)
    ,mCompassData(compassData)
    ,mCompass(compass)
    ,mLeakPin(leakPin)
    ,mBattVoltage(battVoltage)
    ,mLoopDelay( (loopFrequency) ? (unsigned long)(1000/loopFrequency) : 0 )
{ 

}

void RoboSubController::Run()
{
    
    byte recvData[] = { 0, 0, 0 };
    byte sendData[] = { 0, 0, 0 };
    RoboSubControllerData newState;
    unsigned battVoltage;
    uint8_t fireStatus;
    int16_t t1_vel, t2_vel;
    
    unsigned long time, prev;
    
    while( true )
    {
        // Start time
        prev = millis();
        
        // Read Compass Value (divide heading by 2)
        //mCompass.GetData();
        //sendData[0] = mCompassData[CompassData<float>::Heading] / 2.0;
        sendData[0] = 0;
        
        // Read Battery Voltage (subtract 520)
        battVoltage = (unsigned)mBattVoltage.Read();
        sendData[1] = ( battVoltage >= 520 ) ? (( battVoltage <= 750 ) ? battVoltage-520 : 230 ) : 0;
        
        // Get Leak Detection value
        sendData[2] = digitalRead(mLeakPin);
        
        // Send status data:
        mComPort.write(sendData, 3 );
        
        // Get command data:
        mComPort.read(recvData, 3 );
                
        // Copy data 
        newState.Status = recvData[0];
        newState.NunchuckX = recvData[1];
        newState.NunchuckY = recvData[2];
        
        // check reset
        if( newState.Status & 0x80 )
        {
            // call reset function:
            return;
        }
        
        // compute new status
        newState.Status = ( ( mState.Status | newState.Status )&0x07 | ( newState.Status & 0xF8 ) );
        
        // Update firestatus
        fireStatus = ( newState.Status ^ mState.Status )&0x07 | ( newState.Status & 0xF8 );
        
        // update mState
        mState = newState;
        
        // Update Pneumatics
        for( int i = 0; i < 5; ++i )
        {
            mPneumaticData[i] = fireStatus & (1 << i);
        }
        
        // Update Vertical Thrusters (Thrusters 3 and 4)
        for( int i = 2; i < 4; ++i )
        {
            // Up
            if( fireStatus & (1 << 5) )
            {
                if( mThrusterData[2] < 125 ) // 127 - 2 (because up is +2)
                {
                    mThrusterData[2] += 2;
                    mThrusterData[3] += 2;
                }
            }
            else if( fireStatus & (1 << 6) )
            {
                if( mThrusterData[3] > -126 ) // -128 + 2 (because down is -2)
                {
                    mThrusterData[3] -= 2;
                    mThrusterData[2] -= 2;
                }
            }    
        }
        
        // Set Forward Thruster velocities (Thrusters 1 and 2)
        mState.NunchuckX -= 128;    // fix nunchuck coords
        mState.NunchuckY -= 128;    // ""
        t1_vel = ( -mState.NunchuckY ) - mState.NunchuckX;
        t2_vel = ( -mState.NunchuckY ) + mState.NunchuckX;
        
        // Scale velocities
        if( t1_vel > 127 )
        {
            t1_vel = 127;
        }
        else if( t1_vel < -127 )
        {
            t1_vel = -127;
        }
        
        if( t2_vel > 127 )
        {
            t2_vel = 127;
        }
        else if( t2_vel < -127 )
        {
            t2_vel = -127;
        }
        
        // Update Forward Thrusters
        mThrusterData[0] = t1_vel;
        mThrusterData[1] = t2_vel;
        
        // Run Pneumatics
        mPneumaticPlant.CommandAll();
        
        // Run Thrusters
        mThrusterPlant.DriveAll();
        
        // stop time; Check loop rate
        time = millis() - prev;
        if( time < mLoopDelay )
        {
            delay( mLoopDelay - time );
        }
    }
}

#endif //__ROBO_SUB_CONTROLLER_H__
