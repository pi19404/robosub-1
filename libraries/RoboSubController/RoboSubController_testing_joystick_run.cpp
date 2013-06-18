#ifdef ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

#include <RoboSubController/RoboSubController.h>
#include <Jay's_Serial_Data/robosub_control_data.h>
#include <Configurations/DeviceConfig.h>
#include <Logging/LogManager.h>

RoboSubControlData cmd;
char pcDataBuffer[RoboSubControlData::SIZE];

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort)
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i = 0;

    int tempThrusterData;
    bool tempPneumaticData;
    uint16_t depthInches;

    int thrusterDutyCycleBuf[NUM_THRUSTERS];
    int thrusterDirBuf[NUM_THRUSTERS];
//    char clsBuffer[20];

    // this is the CLS pmod that stuff can be printed to
//    CLS mLittleScreen;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    memset(pcDataBuffer, 0, RoboSubControlData::SIZE);
    memset(thrusterDutyCycleBuf, 0, sizeof(thrusterDutyCycleBuf));
    memset(thrusterDirBuf, 0, sizeof(thrusterDirBuf));
//    memset(clsBuffer, 0, sizeof(clsBuffer));

    while( true )
    {
        // Only read when we have the "magic" number
        // this number indicates that we have control data
        _lm.LogStr("waiting for transmission start");
        while(Serial.peek() != RoboSubControlData::MAGIC)
        {
            // Discard this byte,
            // we don't want it
            Serial.read();
        }

        // Wait until all of the control data is in the buffer
        _lm.LogStr("waiting for serial data");
        while(Serial.available() < RoboSubControlData::SIZE);

        // Read byte by byte, since this version of
        // Arduino does not have a readBytes() function for
        // the Serial object.
        _lm.LogStr("reading serial data");
        for(i = 0; i < RoboSubControlData::SIZE; ++i)
        {
            int c = Serial.read();
            pcDataBuffer[i] = static_cast<char>(c);
        }

        // Deserialize the read in bytes into
        // the command objec
        _lm.LogStr("deserializing string");
        cmd.DeserializeFromString(pcDataBuffer);

        // access the individual items in the serialized data structure

        // port fore thruster
        tempThrusterData = cmd.Data.Thruster_Fore_L;
        _lm.LogStrInt("port fore thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[0] = tempThrusterData * (-1);
            thrusterDirBuf[0] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[0] = tempThrusterData;
            thrusterDirBuf[0] = 1;
        }


        // port aft thruster
        tempThrusterData = cmd.Data.Thruster_Aft_L;
        _lm.LogStrInt("port aft thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[1] = tempThrusterData * (-1);
            thrusterDirBuf[1] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[1] = tempThrusterData;
            thrusterDirBuf[1] = 1;
        }


        // starboard fore thruster
        tempThrusterData = cmd.Data.Thruster_Fore_R;
        _lm.LogStrInt("starboard fore thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[2] = tempThrusterData * (-1);
            thrusterDirBuf[2] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[2] = tempThrusterData;
            thrusterDirBuf[2] = 1;
        }


        // starboard aft thruster
        tempThrusterData = cmd.Data.Thruster_Aft_R;
        _lm.LogStrInt("starboard aft thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[3] = tempThrusterData * (-1);
            thrusterDirBuf[3] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[3] = tempThrusterData;
            thrusterDirBuf[3] = 1;
        }


        // port roll thruster
        tempThrusterData = cmd.Data.Thruster_Roll_L;
        _lm.LogStrInt("port roll thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterData * (-1);
            thrusterDirBuf[4] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterData;
            thrusterDirBuf[4] = 1;
        }


        // starboard roll thruster
        tempThrusterData = cmd.Data.Thruster_Roll_R;
        _lm.LogStrInt("starboard roll thruster: ", tempThrusterData);
        if (tempThrusterData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[5] = tempThrusterData * (-1);
            thrusterDirBuf[5] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[5] = tempThrusterData;
            thrusterDirBuf[5] = 1;
        }

        // send the duty cycles and directions to the h bridges
        CU.setThrusters(thrusterDutyCycleBuf, thrusterDirBuf);


        // check torpedo commands
        tempPneumaticData = Torpedo1_Fire;
        if (tempPneumaticData)
        {
            CU.fireTorpedoN(1);
        }

        tempPneumaticData = Torpedo2_Fire;
        if (tempPneumaticData)
        {
            CU.fireTorpedoN(2);
        }


        // check marker dropper commands
        tempPneumaticData = Marker1_Drop
        if (tempPneumaticData)
        {
            CU.dropMarkerN(1);
        }

        tempPneumaticData = Marker2_Drop;
        if (tempPneumaticData)
        {
            CU.dropMarkerN(1);
        }

        // read the depth sensor
        depthInches = IMU.readDepth();


        // "Pretty" print the joystick commands back to the serial line to 
        // ensure the data was sent correctly.
        String toPrint;
        cmd.ToString(toPrint);
        Serial.print(toPrint);

        delay(100);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

