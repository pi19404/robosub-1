#ifdef CLS_IS_I2C

// logs debug messages
#include <Logging/LogManager.h>
#include <string.h>

// for declaration of the CLS class
#include "CLS.h"

// for use of the Arduino "Wire" object
#include <Wire.h>

// these are the addresses for the registers that we will be using
#define CLS_BASE        0x48

// Globals for setting up pmod CLS
// values in Digilent pmod CLS reference manual, pages 2 - 3
char enable_display[] =  {27, '[', '3', 'e', '\0'};
char set_cursor[] =      {27, '[', '1', 'c', '\0'};
char home_cursor[] =     {27, '[', 'j', '\0'};
char wrap_line[] =       {27, '[', '0', 'h', '\0'};
char set_line_two[] =    {27, '[', '1', ';', '0', 'H', '\0'};

/*
The device should only need to be initialized once, and that is on powerup.  We 
are not handling the case where something gets unplugged and has to be 
reconnected.  If that happens, just reconnect the device and power cycle the 
microcontroller, and then things will reinitialize.
*/
CLS::CLS()
{
    writeNBytes(enable_display, strlen(enable_display));
    writeNBytes(set_cursor, strlen(set_cursor));
    writeNBytes(home_cursor, strlen(home_cursor));
    writeNBytes(wrap_line, strlen(wrap_line));
}

void CLS::writeStrToCLS(char *buffer)
{
    if (NULL == buffer)
    {
        // bad argument pointer; just abort
        return;
    }

    writeNBytes(home_cursor, strlen(home_cursor));
    writeNBytes(buffer, strlen(buffer));
}

/*
This is a blind writing function (it is internal to this function, so it is not
directly exposed to external input, and so it doesn't even have a null check), 
so the calling function must be certain of the number of bytes that they want 
to write.
*/
void CLS::writeNBytes(char* buffer, unsigned int numToWrite)
{
    int i;
    
    // start up an instance of the log manager 
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("CLS::writeNBytes - starting transmission to CLS.");
    Wire.beginTransmission(CLS_BASE);
    for (i = 0; i < numToWrite; i++)
    {
        Wire.send(*(buffer + i));
    }
    Wire.endTransmission();
    mLogInstance.LogStr("CLS::writeNBytes - finished transmission to CLS.");
}

#endif  // CLS_IS_I2C
