#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__

#include ".\RoboSubControllerData.h"
#include "..\Utility\NoCopy.h"
#include "..\Configurations\IMCS_Configuration.h"


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
        
        ComPort                                           &mComPort;
        unsigned long                                      mLoopDelay;
};

RoboSubController::RoboSubController(  
                            ComPort                                           &comPort
                           ,float                                              loopFrequency
                          )
    :
     mState()
    ,mComPort(comPort)
    ,mLoopDelay( (loopFrequency) ? (unsigned long)(1000/loopFrequency) : 0 )
{ 

}

void RoboSubController::Run()
{
    
    byte sendData[4] = { 0, 0, 0, 0 };
    byte recvData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    RoboSubControllerData newState;
    uint8_t fireStatus;
    
    unsigned long time, prev;
    
    while( true )
    {
        // Start time
        prev = millis();
        
        // Send status data:
        mComPort.write( sendData, sizeof(sendData)/sizeof(byte) );
        
        // Read Depth
        DEPTH_SENSOR.GetData();
        uint8_t depth = DEPTH_DATA[0];
        
        // Get command data:
        mComPort.read(recvData, sizeof(recvData)/sizeof(byte) );
                            
        sendData[0] = 0;                   
        for( int i = 0; i < sizeof(recvData)/sizeof(byte); ++i )
        {
            if( recvData[i] != i+1 )
            {
                sendData[0] |= (1 << i);
            }
        }
        
        // stop time; Check loop rate
        time = millis() - prev;
        if( time < mLoopDelay )
        {
            delay( mLoopDelay - time );
        }
    }
}

#endif //__ROBO_SUB_CONTROLLER_H__
