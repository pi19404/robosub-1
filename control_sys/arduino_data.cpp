///////////////////////////////////////////////////////////////////////////////
// \file: arduino_data.cpp
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for wrapping ArduinoStatus objects
//
// Major Changes:
// 15-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "arduino_data.h"

// Math stuff for the
// coordinate transformations
#include <cmath>

// Default Constructor.
ArduinoData::ArduinoData()
    :
    ArduinoStatus()
{

}

// Copy Constructor.
ArduinoData::ArduinoData( const ArduinoData& data )
    :
    ArduinoStatus()
{
    Data.Acl_X  = data.Data.Acl_X;
    Data.Acl_Y  = data.Data.Acl_Y;
    Data.Acl_Z  = data.Data.Acl_Z;
    Data.Gyro_X = data.Data.Gyro_X;
    Data.Gyro_Y = data.Data.Gyro_Y;
    Data.Gyro_Z = data.Data.Gyro_Z;
    Data.Depth  = data.Data.Depth;
}

// Assignment Operator
ArduinoData& ArduinoData::operator=( const ArduinoData& rhs )
{
    if( this != &rhs )
    {
        Data.Acl_X  = rhs.Data.Acl_X;
        Data.Acl_Y  = rhs.Data.Acl_Y;
        Data.Acl_Z  = rhs.Data.Acl_Z;
        Data.Gyro_X = rhs.Data.Gyro_X;
        Data.Gyro_Y = rhs.Data.Gyro_Y;
        Data.Gyro_Z = rhs.Data.Gyro_Z;
        Data.Depth  = rhs.Data.Depth;
    }
    return *this;
}

// Equivalence Operator
bool ArduinoData::operator==( const ArduinoData& rhs )
{
    return  ( Data.Acl_X  ==  rhs.Data.Acl_X  )&&
            ( Data.Acl_Y  ==  rhs.Data.Acl_Y  )&&
            ( Data.Acl_Z  ==  rhs.Data.Acl_Z  )&&
            ( Data.Gyro_X ==  rhs.Data.Gyro_X )&&
            ( Data.Gyro_Y ==  rhs.Data.Gyro_Y )&&
            ( Data.Gyro_Z ==  rhs.Data.Gyro_Z )&&
            ( Data.Depth  ==  rhs.Data.Depth  );
}

// Inequivalence Operator
bool ArduinoData::operator!=( const ArduinoData& rhs )
{
    return !(*this == rhs);
}


std::ostream& operator<<( std::ostream& os,
                                 const ArduinoData& rhs )
{
    os << "ArduinoData: \n";
    os << "{\n";
    os << " Acl_X:  " << rhs.Data.Acl_X  << "\n";
    os << " Acl_Y:  " << rhs.Data.Acl_Y  << "\n";
    os << " Acl_Z:  " << rhs.Data.Acl_Z  << "\n";
    os << " Gyro_X: " << rhs.Data.Gyro_X << "\n";
    os << " Gyro_Y: " << rhs.Data.Gyro_Y << "\n";
    os << " Gyro_Z: " << rhs.Data.Gyro_Z << "\n";
    os << " Depth:  " << rhs.Data.Depth  << "\n";
    os << "}" << std::endl;

    return os;
}

