#ifndef __ROBOSUB_CONTROL_COMMAND_H__
#define __ROBOSUB_CONTROL_COMMAND_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_control_command.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: Control System Communication
// Description: utilities for transmitting and receiving control system
//              commands over a wire interface. Also defines a control command.
//
// Major Changes:
// 2-Jun-2013       JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

struct RoboSubControlCommand
{
    RoboSubControlCommand()
        :
        Data()
    {

    }

    void SerializeToString( char *str )
    {
        if( !str ){ return; }
        
        char *str2 = str;
        *str2 = MAGIC;
        ++str2;

        // Thrusters
        uint32_t sz = sizeof(int32_t);
        _Serialize( &Data.Thruster_Fore_L, sz, &str2 );
        _Serialize( &Data.Thruster_Fore_R, sz, &str2 );
        _Serialize( &Data.Thruster_Aft_L, sz, &str2 );
        _Serialize( &Data.Thruster_Aft_R, sz, &str2 );
        _Serialize( &Data.Thruster_Roll_L, sz, &str2 );
        _Serialize( &Data.Thruster_Roll_R, sz, &str2 );

        // Pneumatics
        sz = sizeof(bool);
        _Serialize( &Data.Torpedo1_Fire, sz, &str2 );
        _Serialize( &Data.Torpedo2_Fire, sz, &str2 );
        _Serialize( &Data.Marker1_Drop, sz, &str2 );
        _Serialize( &Data.Marker2_Drop, sz, &str2 );
        _Serialize( &Data.Claw_Latch, sz, &str2 );
    }

    void DeserializeFromString( const char *str )
    {

        const char * str2 = str;
        if( str && (*str != MAGIC) )
        { 
            return; 
        }
        ++str2;

        // Thrusters
        uint32_t sz = sizeof(int32_t);
        _Deserialize( &Data.Thruster_Fore_L, sz, &str2 );
        _Deserialize( &Data.Thruster_Fore_R, sz, &str2 );
        _Deserialize( &Data.Thruster_Aft_L, sz, &str2 );
        _Deserialize( &Data.Thruster_Aft_R, sz, &str2 );
        _Deserialize( &Data.Thruster_Roll_L, sz, &str2 );
        _Deserialize( &Data.Thruster_Roll_R, sz, &str2 );

        // Pneumatics
        sz = sizeof(bool);
        _Deserialize( &Data.Torpedo1_Fire, sz, &str2 );
        _Deserialize( &Data.Torpedo2_Fire, sz, &str2 );
        _Deserialize( &Data.Marker1_Drop, sz, &str2 );
        _Deserialize( &Data.Marker2_Drop, sz, &str2 );
        _Deserialize( &Data.Claw_Latch, sz, &str2 );
    }

    void _Serialize( void *data, uint32_t sz, char **sp )
    {
        char *dptr = static_cast<char*>(data);
        while( sz )
        {
            **sp = *dptr;

            ++dptr;
            ++(*sp);
            --sz;
        }
    }

    void _Deserialize( void *data, uint32_t sz, const char **sp )
    {
        char *dptr = static_cast<char*>(data);
        while( sz )
        {
            *dptr = **sp;

            ++dptr;
            ++(*sp);
            --sz;
        }
    }


    struct DATA 
    {
        DATA()
            :
            Thruster_Fore_L(0),
            Thruster_Fore_R(0),
            Thruster_Aft_L(0),
            Thruster_Aft_R(0),
            Thruster_Roll_L(0),
            Thruster_Roll_R(0),
            Torpedo1_Fire(false),
            Torpedo2_Fire(false),
            Marker1_Drop(false),
            Marker2_Drop(false),
            Claw_Latch(false)
        { }

        int32_t Thruster_Fore_L, // Left-Front Drive Thruster
                Thruster_Fore_R, // Right-Front Drive Thruster
                Thruster_Aft_L,  // Left-Rear Drive Thruster
                Thruster_Aft_R,  // Right-Rear Drive Thruster
                Thruster_Roll_L, // Left Roll Thruster
                Thruster_Roll_R; // Right Roll Thruster

        bool Torpedo1_Fire, // Fire torpedo 1
             Torpedo2_Fire, // Fire torpedo 2
             Marker1_Drop,  // Drop Marker 1
             Marker2_Drop,  // Drop Marker 2
             Claw_Latch;    // Latch the Claw
    } Data;

    static const char MAGIC = 0x22;
    static const uint32_t SIZE = sizeof(DATA);
  
};

#endif //__ROBOSUB_CONTROL_COMMAND_H__

