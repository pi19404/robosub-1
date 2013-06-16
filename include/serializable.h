#ifndef __SERIALIZABLE_H__
#define __SERIALIZABLE_H__
///////////////////////////////////////////////////////////////////////////////
// \file serializable.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: Control System Communication
// Description: Implements utilities for data that needs to be serialized and
//              deserialized. All (de)serialization is done in little endian.
// Major Changes:
// 15-Jun-2013       JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

// Serializable
// \brief Interface class for a (de)serializable object.
class Serializable
{
public:

    // Constructor.
    Serializable(){ }

    // Destructor.
    virtual ~Serializable(){ }

    // SerializeToString
    // \brief Interface for serializing the object
    // \param str - the output string
    // \pre str must have enough storage space.
    // \post str will contain the serialized version of the object.
    virtual void SerializeToString( char *str ) = 0;

    // DeserializeFromString
    // \brief Interface for deserializing the object
    // \param str - the source string
    // \pre str must conatain the serialized version of the object.
    // \post the object will contain the deserialized data from str.
    virtual void DeserializeFromString( const char *str ) = 0;

protected:

    // _Serialize
    // \brief implements the serialization for a data element in little endian.
    // \param data - ptr to the data to serialize
    // \param sz - the size of the data to serialize
    // \param sp - ptr to the destination string
    // \pre the destination string and the data must have size sz.
    // \post the string pointed to by sp will contain the serialized object.
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

    // _Deserialize
    // \brief implements the deserialization to a data element in little endian.
    // \param data - ptr to the destination data
    // \param sz - the size of the destination data
    // \param sp - ptr to the source string
    // \pre the destination data and the source string must have size sz.
    // \post the data pointed to by "data" will contain the deserialized object.
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
};

#endif //__SERIALIZABLE_H__

