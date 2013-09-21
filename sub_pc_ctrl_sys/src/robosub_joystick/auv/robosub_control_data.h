#ifndef __ROBOSUB_CONTROL_DATA_H__
#define __ROBOSUB_CONTROL_DATA_H__
///////////////////////////////////////////////////////////////////////////////
// \file: robosub_control_data.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for converting RoboSubCommand objects into
//              control commands that are sent to the arduino. 
// Major Changes:
// 01-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_command.h"            // command message def
#include "robosub_control_command.h"    // actual data and (de)serialization

#include <stdint.h>
#include <iostream>

struct RoboSubControlData : public RoboSubControlCommand
{
    // Default Constructor.
    RoboSubControlData();

    // Copy Constructor.
    RoboSubControlData( const RoboSubControlData& data );

    // Command Copy Constructor.
    RoboSubControlData( const RoboSubCommand& cmd );

    // Assignment Operator
    RoboSubControlData& operator=( const RoboSubControlData& rhs );

    // Equivalence Operator
    bool operator==( const RoboSubControlData& rhs );

    // Inequivalence Operator
    bool operator!=( const RoboSubControlData& rhs );
    
    // Pretty Print
    friend std::ostream& operator<<( std::ostream& os,
                                     const RoboSubControlData& rhs );

    // FromCommand
    // \brief sets the data from a command
    void FromCommand( const RoboSubCommand* cmd );

    // SerializeToOStream
    // \brief serializes this object to a std::ostream object
    using RoboSubControlCommand::SerializeToString;

    // DeserializeFromIStream
    // \brief deserializes an std::istream into the command data
    using RoboSubControlCommand::DeserializeFromString;
};

#endif //__ROBOSUB_CONTROL_DATA_H__
