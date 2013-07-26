#ifndef __ROBOSUB_COMMAND_H__
#define __ROBOSUB_COMMAND_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_command.h
///////////////////////////////////////////////////////////////////////////////
// Author:      Jay Sidhu (JS) <jaypal.sidhu@gmail.com>
// Project:     RoboSub Joystick Communication
// Description: Implementation class for robosub commands 
//
// Major Changes:
// 26-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include <string>
#include <iostream>

struct RoboSubCommand
{
    static const uint8_t MAGIC = 0x35;

    /*
     * Methods
     */

    // Default Constructor
    RoboSubCommand()
        :
        magic(MAGIC),
        PowerX(0.0),
        PowerY(0.0),
        Heading(0.0),
        Depth(0.0),
        Torpedo1_Fire(false),
        Torpedo2_Fire(false),
        Marker1_Drop(false),
        Marker2_Drop(false),
        Claw_Latch(false)
    { }

    // Copy Constructor
    RoboSubCommand( const RoboSubCommand& c )
        :
        magic(MAGIC),
        PowerX(c.PowerX),
        PowerY(c.PowerY),
        Heading(c.Heading),
        Depth(c.Depth),
        Torpedo1_Fire(c.Torpedo1_Fire),
        Torpedo2_Fire(c.Torpedo2_Fire),
        Marker1_Drop(c.Marker1_Drop),
        Marker2_Drop(c.Marker2_Drop),
        Claw_Latch(c.Claw_Latch)
    { }

    // Assignment Operator
    RoboSubCommand& operator=( const RoboSubCommand& rhs )
    {
        if( *this != rhs )
        {
            PowerX = rhs.PowerX;
            PowerY = rhs.PowerY;
            Heading = rhs.Heading;
            Depth = rhs.Depth;
            Torpedo1_Fire = rhs.Torpedo1_Fire;
            Torpedo2_Fire = rhs.Torpedo2_Fire;
            Marker1_Drop = rhs.Marker1_Drop;
            Marker2_Drop = rhs.Marker2_Drop;
            Claw_Latch = rhs.Claw_Latch;
        }
        return *this;
    }

    // Equivalence Operator
    bool operator==( const RoboSubCommand& rhs )
    {
        return ( PowerX == rhs.PowerX )&&
               ( PowerY == rhs.PowerY )&&
               ( Heading == rhs.Heading )&&
               ( Depth == rhs.Depth )&&
               ( Torpedo1_Fire == rhs.Torpedo1_Fire )&&
               ( Torpedo2_Fire == rhs.Torpedo2_Fire )&&
               ( Marker1_Drop == rhs.Marker1_Drop )&&
               ( Marker2_Drop == rhs.Marker2_Drop )&&
               ( Claw_Latch == rhs.Claw_Latch );
    }

    // Inequivalence operator
    bool operator!=( const RoboSubCommand& rhs )
    {
        return !(*this == rhs);
    }

    // Stream insertion operator
    friend std::ostream& operator<<( std::ostream& os, 
                                     const RoboSubCommand& rhs )
    {
        os << "RoboSubCommand: " << "\n";
        os << "{\n"; 
        os << "  PowerX: " << rhs.PowerX << "\n";
        os << "  PowerY: " << rhs.PowerY << "\n";
        os << "  Heading: " << rhs.Heading << "\n";
        os << "  Depth: " << rhs.Depth << "\n";
        os << "  Torpedo1_Fire: " << rhs.Torpedo1_Fire << "\n";
        os << "  Torpedo2_Fire: " << rhs.Torpedo2_Fire << "\n";
        os << "  Marker1_Drop: " << rhs.Marker1_Drop << "\n";
        os << "  Marker2_Drop: " << rhs.Marker2_Drop << "\n";
        os << "  Claw_Latch: " << rhs.Claw_Latch << "\n";
        os << "}" << std::endl;
        return os;
    }


    void SerializeToString( std::string& str )
    {
        str.assign(reinterpret_cast<char*>(
                    const_cast<RoboSubCommand*>(this))
                   ,sizeof(RoboSubCommand));
    }

    bool DeserializeFromString( const std::string& str )
    {
        std::string copy = str;
        char buffer[copy.size()];
        copy.copy(buffer, copy.size());
        RoboSubCommand *cmd = reinterpret_cast<RoboSubCommand*>(buffer);

        if( cmd->magic == MAGIC )
        {
            *this = *cmd;
            return true;
        }
        return false;
    }

    /*
     * Data Members
     */
    const uint8_t magic;
    float PowerX;
    float PowerY;
    float Heading;
    float Depth;

    bool  Torpedo1_Fire;
    bool  Torpedo2_Fire;
    bool  Marker1_Drop;
    bool  Marker2_Drop;
    bool  Claw_Latch;
};


#endif //__ROBOSUB_COMMAND_H__
