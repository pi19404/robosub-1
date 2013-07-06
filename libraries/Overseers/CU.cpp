#include <Overseers/CU.h>
#include <Configurations/DeviceConfig.h>
#include <Logging/LogManager.h>

CU::CU(void) 
{
    // initializes all devices that need to be controlled (sensors are not
    // controlled; they are read)

    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("CU::CU - starting up thrusters");
    // set the H bridge address for each pair of thrusters
    thrustersPort.init(M1_ADDR);
    thrustersStarboard.init(M2_ADDR);
    thrustersRoll.init(M3_ADDR);

    mLogInstance.LogStr("CU::CU - starting up pneumatics");
    // set the pin numbers for each pneumatic device (this initialization also
    // sets the pins as outputs);
    // see DeviceConfig.h for more details
    mTorpedo1.init(solenoid3Pin);
    mTorpedo2.init(solenoid6Pin);
    mMarker1.init(solenoid1Pin);
    mMarker2.init(solenoid4Pin);
    mClaw1.init(solenoid2Pin);
    mClaw2.init(solenoid5Pin);

    // set all pneumatic devices as ready to fire
    mTorpedo1Ready = true;
    mTorpedo2Ready = true;
    mMarker1Ready = true;
    mMarker2Ready = true;

    mLogInstance.LogStr("CU::CU - finished");
}

bool CU::torpedoNReady(uint8_t torpedoNum)
{
    // checks if the specified torpedo launcher is ready to launch or not

    // sanitize the data;
    // assume that the torpedoes will be called 1 and 2 (not 0 and 1)

    LogManager& mLogInstance = LogManager::GetInstance();

    if (1 == torpedoNum)
    {
        if (mTorpedo1Ready)
        {
            mLogInstance.LogStr("CU::torpedoNReady - torpedo 1 ready");
            return true;
        }
        else
        {
            mLogInstance.LogStr("CU::torpedoNReady - torpedo 1 not ready");
            return false;
        }
    }
    else if (2 == torpedoNum)
    {
        if (mTorpedo2Ready)
        {
            mLogInstance.LogStr("CU::torpedoNReady - torpedo 2 ready");
            return true;
        }
        else
        {
            mLogInstance.LogStr("CU::torpedoNReady - torpedo 2 not ready");
            return false;
        }
    }
    else
    {
        // not good data;
        // return false to be safe
        mLogInstance.LogStrInt("CU::torpedoNReady - torpedo number invalid: ", torpedoNum);
        return false;
    }
}

void CU::fireTorpedoN(uint8_t torpedoNum)
{
    // commands the specified torpedo launcher to fire its payload if ready

    // Note: must be commanded by dropped a high bit (1) into the proper place
    // in the pneumatic device's data;
    // thus, this is implementation dependent

    LogManager& mLogInstance = LogManager::GetInstance();

    // check if torpedo is read to fire
    if (!torpedoNReady(torpedoNum))
    {
        // torpedo not ready or bad torpedo number;
        // do nothing
    }
    else
    {
        if (1 == torpedoNum)
        {
            mLogInstance.LogStr("CU::fireTorpedoN - Firing torpedo 1");
            mTorpedo1.activate();
            delay(500);
            mTorpedo1.deactivate();
            mTorpedo1Ready = false;
        }
        else if (2 == torpedoNum)
        {
            mLogInstance.LogStr("CU::fireTorpedoN - Firing torpedo 2");
            mTorpedo2.activate();
            delay(500);
            mTorpedo2.deactivate();
            mTorpedo2Ready = false;
        }
        else
        {
            mLogInstance.LogStr("CU::fireTorpedoN - Well.  This is embarasing.  \
The first check should have caught this.");
        }
    }
}

bool CU::markerNReady(uint8_t markerNum)
{
    // checks if the specified marker is ready to drop its payload or not

    // sanitize the data;
    // assume that the torpedoes will be called 1 and 2 (not 0 and 1)

    LogManager& mLogInstance = LogManager::GetInstance();

    if (1 == markerNum)
    {
        if (mMarker1Ready)
        {
            mLogInstance.LogStr("CU::markerNReady - marker 1 ready");
            return true;
        }
        else
        {
            mLogInstance.LogStr("CU::markerNReady - marker 1 not ready");
            return false;
        }
    }
    else if (2 == markerNum)
    {
        if (mMarker2Ready)
        {
            mLogInstance.LogStr("CU::markerNReady - marker 2 ready");
            return true;
        }
        else
        {
            mLogInstance.LogStr("CU::markerNReady - marker 2 not ready");
            return false;
        }
    }
    else
    {
        // not good data;
        // return false to be safe
        mLogInstance.LogStrInt("CU::markerNReady - marker number invalid: ", markerNum);
        return false;
    }
}

void CU::dropMarkerN(uint8_t markerNum)
{
    // commands the specified marker to drop its payload if it is ready

    // Note: must be commanded by dropped a high bit (1) into the proper place
    // in the pneumatic device's data;
    // thus, this is implementation dependent

    LogManager& mLogInstance = LogManager::GetInstance();

    // sanitize the data
    if (!markerNReady(markerNum))
    {
        // marker not ready or bad marker number;
        // do nothing
    }
    else
    {
        if (1 == markerNum)
        {
            mLogInstance.LogStr("CU::dropMarkerN - Dropping marker 1");
            mMarker1.activate();
            delay(500);
            mMarker1.deactivate();
            mMarker1Ready = false;
        }
        else if (2 == markerNum)
        {
            mLogInstance.LogStr("CU::dropMarkerN - Dropping marker 2");
            mMarker2.activate();
            delay(500);
            mMarker2.deactivate();
            mMarker2Ready = false;
        }
        else
        {
            mLogInstance.LogStr("CU::dropMarkerN - Well.  This is embarrsing.  \
The first check should have caught this.");
        }
    }
}
	
bool CU::clawOpen()
{
    // ?? necessary? to do??
    return false;
}

uint8_t CU::thrusterNState(uint8_t thrusterNum)
{
    // ??necessary? to do??
    return 0;
}

void CU::setThrusters(int *dutyCycles, int *motorDirections)
{
    // update all thrusters with the provided array of thruster data

    // 'pwmThrusterInfo' should be a pointer to an array of thruster data of
    // size 'NUM_THRUSTERS';
    // cross your fingers and hope it is used as intended

    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("CU::updateThrusters - Updating thrusters.");

    mLogInstance.LogStrInt("CU::updateThrusters - Port fore thruster duty = ", dutyCycles[0]);
    mLogInstance.LogStrInt("CU::updateThrusters - Port fore dir = ", motorDirections[0]);
    thrustersPort.setMotor(1, dutyCycles[0], motorDirections[0]);

    mLogInstance.LogStrInt("CU::updateThrusters - Port aft thruster duty = ", dutyCycles[1]);
    mLogInstance.LogStrInt("CU::updateThrusters - Port aft dir = ", motorDirections[1]);
    thrustersPort.setMotor(2, dutyCycles[1], motorDirections[1]);

    mLogInstance.LogStrInt("CU::updateThrusters - Starboard fore thruster duty = ", dutyCycles[2]);
    mLogInstance.LogStrInt("CU::updateThrusters - Starboard fore dir = ", motorDirections[2]);
    thrustersStarboard.setMotor(1, dutyCycles[2], motorDirections[2]);

    mLogInstance.LogStrInt("CU::updateThrusters - Starboard aft thruster duty = ", dutyCycles[3]);
    mLogInstance.LogStrInt("CU::updateThrusters - Starboard aft dir = ", motorDirections[3]);
    thrustersStarboard.setMotor(2, dutyCycles[3], motorDirections[3]);

    mLogInstance.LogStrInt("CU::updateThrusters - Roll port thruster duty = ", dutyCycles[4]);
    mLogInstance.LogStrInt("CU::updateThrusters - Roll port dir = ", motorDirections[4]);
    thrustersRoll.setMotor(1, dutyCycles[4], motorDirections[4]);

    mLogInstance.LogStrInt("CU::updateThrusters - Roll port thruster duty = ", dutyCycles[5]);
    mLogInstance.LogStrInt("CU::updateThrusters - Roll port dir = ", motorDirections[5]);
    thrustersRoll.setMotor(2, dutyCycles[5], motorDirections[5]);
}

void CU::stopThrusters(void)
{
    // drives all thrusters with a 0 duty cycle

    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("CU::stopThrusters - Stopping thrusters.");

    mLogInstance.LogStr("CU::stopThrusters - Stopping port driver thrusters");
    thrustersPort.stopMotor(1);
    thrustersPort.stopMotor(2);

    mLogInstance.LogStr("CU::stopThrusters - Stopping starboard drive thrusters");
    thrustersStarboard.stopMotor(1);
    thrustersStarboard.stopMotor(2);

    mLogInstance.LogStr("CU::stopThrusters - Stopping roll thrusters");
    thrustersRoll.stopMotor(1);
    thrustersRoll.stopMotor(2);
}

