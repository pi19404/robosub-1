#ifndef __ROBOSUB_CONTROL_DATA_H__
#define __ROBOSUB_CONTROL_DATA_H__
///////////////////////////////////////////////////////////////////////////////
// \file: robosub_control_data.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for control data.
//
// Major Changes:
// 02-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <WString.h>
#include "robosub_control_command.h"
#include <stdint.h>

struct RoboSubControlData : public RoboSubControlCommand
{   
    // Default Constructor
     RoboSubControlData()
       :
       RoboSubControlCommand()
     { }

    // SerializeToString
    // \brief serializes this object to a string
    using RoboSubControlCommand::SerializeToString;
    
    // DeserializeFromString
    // \brief deserializes a string into a control command and copies
    //        it to this object
    // \return true if successful, false otherwise
    using RoboSubControlCommand::DeserializeFromString;
    
    // Pretty print
    // \brief Converts to String Object for Printing
    void ToString(String& str)
    {
    	str +=  "";
    	str +=  "RoboSubControlData: \n";
    	str +=  "{\n";
    	str +=  "  Thruster_Fore_L: " + String(Data.Thruster_Fore_L) + "\n";
    	str +=  "  Thruster_Fore_R: " + String(Data.Thruster_Fore_R) + "\n";
    	str +=  "  Thruster_Aft_L: " + String(Data.Thruster_Aft_L) + "\n";
    	str +=  "  Thruster_Aft_R: " + String(Data.Thruster_Aft_R) + "\n";
    	str +=  "  Thruster_Roll_L: " + String(Data.Thruster_Roll_L) + "\n";
    	str +=  "  Thruster_Roll_R: " + String(Data.Thruster_Roll_R) + "\n";
    	str +=  "  Torpedo1_Fire: " + String(Data.Torpedo1_Fire) + "\n";
    	str +=  "  Torpedo2_Fire: " + String(Data.Torpedo2_Fire) + "\n";
    	str +=  "  Marker1_Drop: " + String(Data.Marker1_Drop) + "\n";
    	str +=  "  Marker2_Drop: " + String(Data.Marker2_Drop) + "\n";
    	str +=  "  Claw_Latch: " + String(Data.Claw_Latch) + "\n";
    	str += "}\n";
    }
};

#endif //__ROBOSUB_CONTROL_DATA_H__
