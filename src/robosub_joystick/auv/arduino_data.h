#ifndef __ARDUINO_DATA_H__
#define __ARDUINO_DATA_H__
///////////////////////////////////////////////////////////////////////////////
// \file: arduino_data.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for wrapping ArduinoStatus objects, to add
//              features to them.
// 
// Major Changes:
// 15-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "arduino_status.h"    // actual data and (de)serialization

#include <stdint.h>
#include <iostream>

struct ArduinoData : public ArduinoStatus
{
    // Default Constructor.
    ArduinoData();

    // Copy Constructor.
    ArduinoData( const ArduinoData& data );

    // Assignment Operator
    ArduinoData& operator=( const ArduinoData& rhs );

    // Equivalence Operator
    bool operator==( const ArduinoData& rhs );

    // Inequivalence Operator
    bool operator!=( const ArduinoData& rhs );
    
    // Pretty Print
    friend std::ostream& operator<<( std::ostream& os,
                                     const ArduinoData& rhs );

    // SerializeToOStream
    // \brief serializes this object into a c-string
    using ArduinoStatus::SerializeToString;

    // DeserializeFromIStream
    // \brief deserializes this object from a c-string
    using ArduinoStatus::DeserializeFromString;
};

#endif //__ARDUINO_DATA_H__
