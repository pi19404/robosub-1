#ifdef ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

#include <RoboSubController/RoboSubController.h>
#include <Jay's_Serial_Data/robosub_control_data.h> // for commands from PC
#include <Jay's_Serial_Data/arduino_data.h>         // for sensor data to PC
#include <Configurations/DeviceConfig.h>
#include <Logging/LogManager.h>


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
    ACCEL_DATA myAccelData;
    GYRO_DATA myGyroData;

    // this buffer stores incoming data from the sub's PC, which tell the sub 
    // what to do
    char pcCmdDataBuffer[RoboSubControlData::SIZE];

    // this buffer stores a serialized version of a bunch of sensor data so 
    // that it can be sent over the serial port to the sub's pc
    char subSensorDataBuffer[ArduinoData::SIZE];

    // this structure will store the non-serialized command data and has the 
    // functions to deserialize a serialized buffer of command data into 
    // non-serialized command data and store those commands in itself and (if 
    // necessary) serialize its own command data back into serialized char 
    // buffer
    RoboSubControlData pcCmdData;

    // this structure will store the non-serialized sensor data and has the 
    // functions to serialize this data into a serialized character buffer and 
    // (if necessary) deserialize a serialized buffer of sensor data back into 
    // non-serialized sensor data and store that data in itself
    ArduinoData subSensorData;

    // these buffers store duty cycles and directions that are retrieved from
    // the deserialized command data sent from the sub's PC
    int thrusterDutyCycleBuf[NUM_THRUSTERS];
    int thrusterDirBuf[NUM_THRUSTERS];

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    memset(pcCmdDataBuffer, 0, RoboSubControlData::SIZE);
    memset(thrusterDutyCycleBuf, 0, sizeof(thrusterDutyCycleBuf));
    memset(thrusterDirBuf, 0, sizeof(thrusterDirBuf));
    memset(thrusterDutyCycleBuf, 0, sizeof(thrusterDutyCycleBuf));
    memset(thrusterDirBuf, 0, sizeof(thrusterDirBuf));

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
            pcCmdDataBuffer[i] = static_cast<char>(c);
        }

        // Verify received data is valid
        if( !RoboSubControlData::SerializedIsValid(pcCmdDataBuffer, RoboSubControlData::SIZE) )
        {
            // The data was malformed, so, discard it
            _lm.LogStr("error - control Data malformed, retrying...");
            continue;
        }

        // Deserialize the read in bytes into
        // the command objec
        _lm.LogStr("deserializing string");
        pcCmdData.DeserializeFromString(pcCmdDataBuffer);

        // access the individual items in the serialized data structure

        // port fore thruster
        tempThrusterData = pcCmdData.Data.Thruster_Fore_L;
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
        tempThrusterData = pcCmdData.Data.Thruster_Aft_L;
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
        tempThrusterData = pcCmdData.Data.Thruster_Fore_R;
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
        tempThrusterData = pcCmdData.Data.Thruster_Aft_R;
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
        tempThrusterData = pcCmdData.Data.Thruster_Roll_L;
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
        tempThrusterData = pcCmdData.Data.Thruster_Roll_R;
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
        mCU.setThrusters(thrusterDutyCycleBuf, thrusterDirBuf);


        // check torpedo commands
        tempPneumaticData = pcCmdData.Data.Torpedo1_Fire;
        _lm.LogStrInt("torpedo 1: ", tempPneumaticData);
        if (tempPneumaticData)
        {
            mCU.fireTorpedoN(1);
        }

        tempPneumaticData = pcCmdData.Data.Torpedo2_Fire;
        _lm.LogStrInt("torpedo 2: ", tempPneumaticData);
        if (tempPneumaticData)
        {
            mCU.fireTorpedoN(2);
        }


        // check marker dropper commands
        tempPneumaticData = pcCmdData.Data.Marker1_Drop;
        _lm.LogStrInt("marker 1: ", tempPneumaticData);
        if (tempPneumaticData)
        {
            mCU.dropMarkerN(1);
        }

        tempPneumaticData = pcCmdData.Data.Marker2_Drop;
        _lm.LogStrInt("marker 2: ", tempPneumaticData);
        if (tempPneumaticData)
        {
            mCU.dropMarkerN(1);
        }

        // check claw command
        tempPneumaticData = pcCmdData.Data.Claw_Latch;
        _lm.LogStrInt("claw: ", tempPneumaticData);
        if (tempPneumaticData)
        {
            mCU.clawOpen();
        }
        else
        {
            mCU.clawClose();
        }
        
        // read the depth sensor
        depthInches = mIMU.readDepth();

        // read the accelerometer
        mIMU.getAccelAll(&myAccelData);

        // read the gyroscope
        mIMU.getGyroAll(&myGyroData);
/*
        // read the accelerometer
        myAccelData.X = 1.1;
        myAccelData.Y = 2.2;
        myAccelData.Z = -3.3;

        // read the gyroscope
        myGyroData.X = 4.4;
        myGyroData.Y = -5.5;
        myGyroData.Z = 6.6;
*/
        // serialize the sensor data and send it back to the sub's PC
        String toSubPC;
        subSensorData.Data.Acl_X = myAccelData.X;
        subSensorData.Data.Acl_Y = myAccelData.Y;
        subSensorData.Data.Acl_Z = myAccelData.Z;
        subSensorData.Data.Gyro_X = myGyroData.X;
        subSensorData.Data.Gyro_Y = myGyroData.Y;
        subSensorData.Data.Gyro_Z = myGyroData.Z;
        subSensorData.Data.Depth = depthInches;
        subSensorData.SerializeToString(subSensorDataBuffer);

        // the arduino Serial write(buf, len) function requires a uint8_t* for
        // the buffer pointer, but Jay's serialization code requires char*, so
        // I decided to declare the buffer as char* and then cast it to 
        // uint8_t* here
        Serial.write((uint8_t *)subSensorDataBuffer, ArduinoData::SIZE);

        // "Pretty" print the joystick commands back to the serial line to 
        // ensure the data was sent correctly.
//        String toPrint;
//        pcCmdData.ToString(toPrint);
//        Serial.print(toPrint);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

