#ifndef __ARDUINO_DATA_H__
#define __ARDUINO_DATA_H__
///////////////////////////////////////////////////////////////////////////////
// \file: robosub_control_data.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for arduino status
//
// Major Changes:
// 02-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <WString.h>
#include "arduino_status.h"
#include <stdint.h>

struct ArduinoData : public ArduinoStatus
{   
    // Default Constructor
     ArduinoData()
       :
       ArduinoStatus()
     { }

    // SerializeToString
    // \brief serializes this object to a string
    using ArduinoStatus::SerializeToString;
    
    // DeserializeFromString
    // \brief deserializes a string into a control command and copies
    //        it to this object
    using ArduinoStatus::DeserializeFromString;
    
    // Pretty print
    // \brief Converts to String Object for Printing
    void ToString(String& str)
    {
    	str +=  "";
    	str +=  "ArduinoData: \n";
    	str +=  "{\n";
    	str +=  "  Accelerometer X: " + _FloatToString(Data.Acl_X , 2) + "\n";
    	str +=  "  Accelerometer Y: " + _FloatToString(Data.Acl_Y , 2) + "\n";
    	str +=  "  Accelerometer Z: " + _FloatToString(Data.Acl_Z , 2) + "\n";
    	str +=  "  Gyroscope     X: " + _FloatToString(Data.Gyro_X, 2) + "\n";
    	str +=  "  Gyroscope     Y: " + _FloatToString(Data.Gyro_Y, 2) + "\n";
    	str +=  "  Gyroscope     Z: " + _FloatToString(Data.Gyro_Z, 2) + "\n";
    	str +=  "  Depth          : " + _FloatToString(Data.Depth , 2) + "\n";
    	str += "}\n";
    }

    // _FloatToString
    // \brief helper method for converting a float to a string
    // \param fl - the floating point number to convert
    // \param precision - the number of decimal places
    // \return the string representation of the floating point number
    String _FloatToString(float fl, unsigned int precision)
    {
        String str;
        str = (int)fl;
        str += ".";

        for( int i = 0; i < precision; ++i )
        {
            // remove the int part
            fl -= (int)fl;

            // append the remainder
            fl *= 10.0;
            str += (int)fl;
        } 
        return str;
    }
};

#endif //__ARDUINO_DATA_H__
