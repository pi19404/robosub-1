#ifndef __CU_H__
#define __CU_H__

// for debug logging
#include <string.h>

// Note: Local for CU.h only!
#define LOG_STRING_LEN 128

// for our custom vectors
#include <Math/Vector.h>

// a configuration file with device addresses and number of certain devices
#include <Configurations/DeviceConfig.h>

// the control unit must be unique (??noCopy does not guarantee this??)
#include <Utility/NoCopy.h>

// for pneumatic control
#include <Pneumatics/pneumaticDevice.h>

// for thruster control
#include <Thruster/thrusterPair.h>


// control unit
class CU 
    :
    public NoCopy
{
public:
	CU();

	virtual bool torpedoNReady(uint8_t torpedoNum);
    virtual void fireTorpedoN(uint8_t torpedoNum);

	virtual bool markerNReady(uint8_t markerNum);
    virtual void dropMarkerN(uint8_t markerNum);
	
	virtual bool clawOpen();

	virtual uint8_t thrusterNState(uint8_t thrusterNum);
    virtual void setThrusters(int *dutyCycles, int *motorDirections);
    virtual void stopThrusters();

private:
    // for logging messages (used during debugging), we need a local reference
    // to the singleton log manager and a buffer that can be used to construct
    // a string with variable data
    char mLogString[LOG_STRING_LEN];

    thrusterPair thrustersPort;
    thrusterPair thrustersStarboard;
    thrusterPair thrustersRoll;

    pDev mTorpedo1;
    pDev mTorpedo2;
    pDev mMarker1;
    pDev mMarker2;
    
    // these track the state of pneumatic actiuators that are only supposed to 
    // fire one time
    bool mTorpedo1Ready;
    bool mTorpedo2Ready;
    bool mMarker1Ready;
    bool mMarker2Ready;
};



#endif // __CU_H__
