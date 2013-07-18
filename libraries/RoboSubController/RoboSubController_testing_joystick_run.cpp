#ifdef ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

#include <RoboSubController/RoboSubController.h>
#include <Jay's_Serial_Data/robosub_control_data.h> // for commands from PC
#include <Jay's_Serial_Data/arduino_data.h>         // for sensor data to PC
#include <Configurations/DeviceConfig.h>
#include <Logging/LogManager.h>

// undefine this to enable the use of the depth controller in Run(), but note 
// that the command data from the PC MUST HAVE A TARGET DEPTH FIELD FIRST
#define ROLL_THRUSTER_MANUAL_CONTROL

// this value is used to cap the target depth at nothing dangeously deep for 
// sub; after all, a calculation gone bad could cause the sub to think that the 
// target depth is 100 fathoms, which is way beyond its capabilities, so do the
// sane thing and sanitize the data
static const int MAX_SUB_DEPTH_INCHES = 240;


RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort)
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

// this function is not a method of RoboSubController 
/*
Ideally, this code would be implemented in the Run() method, but that code is 
already busy and organized and adding a proportional controller to the mix 
would make things messy with the variables all the way at the top and the 
controller way down at the bottom.  So the controller code was placed in its 
own function here, which should be considered an extension of Run() and not a 
method that should be called independently of Run().

Note: since this function is not a method of RoboSubController, it cannot 
access the CU for thruster control or the IMU to read depth, so this function 
takes a current depth and a target depth, calculates the duty cycle to apply to 
the roll thrusters, and returns a duty cycle.  

That's it.  It has no responsibility to drive the thrusters.
*/
static int depthController(int currentDepthInches, int targetDepthInches)
{
    // this non-blocking function implements a proportional controller to take
    // the sub to a target depth
    /*
    In the current design, the sub waits for commands from the PC, and when it 
    gets them, it executes them, reads sensor data, seriealizes it and spits it 
    back to the PC, then waits for commands again.  This means that the loop 
    frequency is entirely dependent upon the PC, which, at the time of this 
    writing (7-18-2013), sends commands 10 times per second when the joystick 
    has something to send.  Since the timing is independent of this system, I 
    do not have access to any timing scheme that grants me a known time 
    differential between one loop iteration and the next, and therefore I 
    cannot implement an integral or derivative calculation into the this 
    controller.  All I have is a simple, linear control curve for roll thruster 
    duty cycles.  It isn't ideal, but it will work for now.  

    Just calculate the difference between the target depth and the current 
    depth, multiply that number by a hand-wavy-approximation constant to 
    convert the error into desired duty cycle, and the return that value.
    */
    const int kProportional = 5;
    int errDepthInches;
    int newDutyCycle;

    LogManager& mLogInstance = LogManager::GetInstance();

    // calculate the error
    /*
    The convention here is that depth is a positive value.  It is not altitude, 
    even though this subs depth sensor could be used for that.  The sensor 
    measures absolute pressure, and we want to use it for depth calculations, 
    so when we read it, we convert the data to depth below the water.  High 
    postive values means that we are really deep.  Low positive values indicate 
    that we are near the surface of the water, and negative values mean that we 
    are above the water.  These concepts require calibrations that are subject 
    to change from Pullman to the competition in San Diego, CA.

    In this error calculation, a positive error means that we are shallower 
    than the target depth and need to go down, and a negative error means that 
    we are below target depth and need to go up.

    Also, cap the target depth at 20'.  We don't want a bad signal to try to 
    take the sub to 100 fathoms or something.
    */
    if (targetDepthInches < 0)
    {
        // want to go above the water, so stop it at 0
        targetDepthInches = 0;
    }
    else if (targetDepthInches > MAX_SUB_DEPTH_INCHES)
    {
        targetDepthInches = MAX_SUB_DEPTH_INCHES;
    }
    errDepthInches = targetDepthInches - currentDepthInches;
    mLogInstance.LogStrInt("depthController - error: ", errDepthInches);

    // now calculate the new duty cycle for the roll thrusters and apply it
    /*
    No data sanitation necessary here because the thrusters handle the 
    constriction of data to within acceptable parameters.  If I put data 
    sanitation here then the responsibility of duty cycle sanitation would be 
    split between this function and the thrusters.  I don't want the 
    responsibility to be anywhere but the thrusters.
    */
    newDutyCycle = errDepthInches * kProportional;
    mLogInstance.LogStrInt("depthController - new duty cycle: ", newDutyCycle);

    // now give the duty cycle to Run() so it can do something with it
    return newDutyCycle;
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i = 0;

    bool durpy = false;

    int tempThrusterCmdData;
    bool tempPneumaticCmdData;
    uint16_t depthCurrentInches;
    uint16_t depthTargetInches;
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
        _lm.LogStrInt("max bytes: ", RoboSubControlData::NO_MAGIC_SIZE);
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
/*
        if( !RoboSubControlData::SerializedIsValid(pcCmdDataBuffer, RoboSubControlData::SIZE) )
        {
            // The data was malformed, so, discard it
            _lm.LogStr("error - control Data malformed, retrying...");

            if (durpy)
            {
                mCU.clawOpen();
                durpy = false;
            }
            else
            {
                mCU.clawClose();
                durpy = true;
            }

            continue;
        }
*/

        // Deserialize the read in bytes into
        // the command objec
        _lm.LogStr("deserializing string");
        pcCmdData.DeserializeFromString(pcCmdDataBuffer);

        // access the individual items in the serialized data structure

        // port fore thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Fore_L;
        _lm.LogStrInt("port fore thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[0] = tempThrusterCmdData * (-1);
            thrusterDirBuf[0] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[0] = tempThrusterCmdData;
            thrusterDirBuf[0] = 1;
        }


        // port aft thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Aft_L;
        _lm.LogStrInt("port aft thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[1] = tempThrusterCmdData * (-1);
            thrusterDirBuf[1] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[1] = tempThrusterCmdData;
            thrusterDirBuf[1] = 1;
        }


        // starboard fore thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Fore_R;
        _lm.LogStrInt("starboard fore thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[2] = tempThrusterCmdData * (-1);
            thrusterDirBuf[2] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[2] = tempThrusterCmdData;
            thrusterDirBuf[2] = 1;
        }


        // starboard aft thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Aft_R;
        _lm.LogStrInt("starboard aft thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[3] = tempThrusterCmdData * (-1);
            thrusterDirBuf[3] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[3] = tempThrusterCmdData;
            thrusterDirBuf[3] = 1;
        }

#ifdef ROLL_THRUSTER_MANUAL_CONTROL
        // port roll thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Roll_L;
        _lm.LogStrInt("port roll thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterCmdData * (-1);
            thrusterDirBuf[4] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterCmdData;
            thrusterDirBuf[4] = 1;
        }


        // starboard roll thruster
        tempThrusterCmdData = pcCmdData.Data.Thruster_Roll_R;
        _lm.LogStrInt("starboard roll thruster: ", tempThrusterCmdData);
        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[5] = tempThrusterCmdData * (-1);
            thrusterDirBuf[5] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[5] = tempThrusterCmdData;
            thrusterDirBuf[5] = 1;
        }
#else
        // read the depth sensor
        /*
        We read it earlier than the other sensors just for the sake of the 
        depth controller.
        */
        depthCurrentInches = mIMU.readDepth();
        depthTargetInches = pcCmdData.Data.target_depth;
        tempThrusterCmdData = 
            depthController(depthCurrentInches, depthTargetInches);

        if (tempThrusterCmdData < 0)
        {
            // negative duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterCmdData * (-1);
            thrusterDutyCycleBuf[5] = tempThrusterCmdData * (-1);
            thrusterDirBuf[4] = -1;
            thrusterDirBuf[5] = -1;
        }
        else
        {
            // positive or zero duty cycle requested
            thrusterDutyCycleBuf[4] = tempThrusterCmdData;
            thrusterDutyCycleBuf[5] = tempThrusterCmdData;
            thrusterDirBuf[4] = 1;
            thrusterDirBuf[5] = 1;
        }
#endif

        // send the duty cycles and directions to the h bridges
        mCU.setThrusters(thrusterDutyCycleBuf, thrusterDirBuf);


        // check torpedo commands
        tempPneumaticCmdData = pcCmdData.Data.Torpedo1_Fire;
        _lm.LogStrInt("torpedo 1: ", tempPneumaticCmdData);
        if (tempPneumaticCmdData)
        {
            mCU.fireTorpedoN(1);
        }

        tempPneumaticCmdData = pcCmdData.Data.Torpedo2_Fire;
        _lm.LogStrInt("torpedo 2: ", tempPneumaticCmdData);
        if (tempPneumaticCmdData)
        {
            mCU.fireTorpedoN(2);
        }


        // check marker dropper commands
        tempPneumaticCmdData = pcCmdData.Data.Marker1_Drop;
        _lm.LogStrInt("marker 1: ", tempPneumaticCmdData);
        if (tempPneumaticCmdData)
        {
            mCU.dropMarkerN(1);
        }

        tempPneumaticCmdData = pcCmdData.Data.Marker2_Drop;
        _lm.LogStrInt("marker 2: ", tempPneumaticCmdData);
        if (tempPneumaticCmdData)
        {
            mCU.dropMarkerN(1);
        }

        // check claw command
        tempPneumaticCmdData = pcCmdData.Data.Claw_Latch;
        _lm.LogStrInt("claw: ", tempPneumaticCmdData);
        if (tempPneumaticCmdData)
        {
            mCU.clawOpen();
        }
        else
        {
            mCU.clawClose();
        }

        // now grab the sensor data, serialize it, and spit it back to the PC

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
        subSensorData.Data.Depth = depthCurrentInches;
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

        // ??should there be a delay??
//        delay(100);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN

