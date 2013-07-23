#ifndef __ARDUINO_STATUS_H__
#define __ARDUINO_STATUS_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_control_command.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: Control System Communication
// Description: utilities for transmitting and receiving arduino status
//              messages over a wire interface. Also defines arduino status.
//
// Major Changes:
// 16-Jun-2013       JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "serializable.h"

struct ArduinoStatus : public Serializable
{
    // Constructor.
    ArduinoStatus()
        :
        Serializable(),
        Data()
    {

    }
   
    // Destructor.
    ~ArduinoStatus(){ }

    
    // SerializeToString
    // \brief Interface for serializing the object
    // \param str - the output string
    // \pre str must have enough storage space.
    // \post str will contain the serialized version of the object.
    void SerializeToString( char *str )
    {
        if( !str ){ return; }
     
        // Write magic number   
        char *str2 = str;
        *str2 = MAGIC;
        ++str2;

        // Accelerometer and Gyroscope
        uint32_t sz = sizeof(float);
        _Serialize( &Data.Acl_X , sz, &str2 );
        _Serialize( &Data.Acl_Y , sz, &str2 );
        _Serialize( &Data.Acl_Z , sz, &str2 );
        _Serialize( &Data.Gyro_X, sz, &str2 );
        _Serialize( &Data.Gyro_Y, sz, &str2 );
        _Serialize( &Data.Gyro_Z, sz, &str2 );

        // Depth Sensor
        sz = sizeof(uint32_t);
        _Serialize( &Data.Depth, sz, &str2 );
    }

    // DeserializeFromString
    // \brief Interface for deserializing the object
    // \param str - the source string
    // \pre str must conatain the serialized version of the object.
    // \post the object will contain the deserialized data from str.
    void DeserializeFromString( const char *str )
    {
        // Check magic number
        const char * str2 = str;
        if( str && (*str != MAGIC) )
        { 
            return; 
        }
        ++str2;

        // Accelerometer and Gyroscope
        uint32_t sz = sizeof(float);
        _Deserialize( &Data.Acl_X , sz, &str2 );
        _Deserialize( &Data.Acl_Y , sz, &str2 );
        _Deserialize( &Data.Acl_Z , sz, &str2 );
        _Deserialize( &Data.Gyro_X, sz, &str2 );
        _Deserialize( &Data.Gyro_Y, sz, &str2 );
        _Deserialize( &Data.Gyro_Z, sz, &str2 );

        // Depth Sensor
        sz = sizeof(uint32_t);
        _Deserialize( &Data.Depth, sz, &str2 );
    }

    struct DATA 
    {
        DATA()
            :
            Acl_X(0.0),
            Acl_Y(0.0),
            Acl_Z(0.0),
            Gyro_X(0.0),
            Gyro_Y(0.0),
            Gyro_Z(0.0),
            Depth(0)
        { }

        float Acl_X,  // Accelerometer X-axis value
              Acl_Y,  // Accelerometer Y-axis value
              Acl_Z,  // Accelerometer Z-axis value
              Gyro_X, // Gyroscope X-axis value
              Gyro_Y, // Gyroscope Y-axis value
              Gyro_Z; // Gyroscope Z-axis value
 
     uint32_t Depth;  // The Depth sensor reading

    } Data;

    static const char MAGIC = 0x25;
    static const uint32_t SIZE = sizeof(MAGIC) + sizeof(DATA);
};

#endif //__ARDUINO_STATUS_H__

