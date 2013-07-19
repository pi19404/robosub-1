#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_contorller.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Control System
// Description: Class spec for RoboSubController, which provides the Control
//                  System API to the AI. 
//
// Major Changes:
// 09-Jul-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_control_data.h" // (To   Arduino) Command
#include "arduino_data.h"         // (From Arduino) Status

#include <string>
#include <boost/asio.hpp>

class ThrustMode;
class PneumMode;

// RoboSubController
// \brief Converts command messages into control messages.
class RoboSubController
{
public:
    
    // Constructor
    RoboSubController();

    // Destructor
    ~RoboSubController();

    // Run
    // \brief causes the control system to open the serial port and 
    // start listening for measurement data.
    // \throw runtime_error if port fails to open
    void Run( std::string comPort, std::string baudRate );

    // AttachDataReceivedCallback
    // \brief attaches a data ready handler for reading measurement data
    void AttachDataReceivedCallback( void (&)(const ArduinoData&) );

    // AttachDataSentCallback
    // \brief attaches a data ready handler for reading measurement data
    void AttachDataSentCallback( void (&)(const RoboSubCommand&) );

    // SendCommand
    // \brief issues a command to the Arduino and gets the response
    void SendCommand();

    // Command
    // \brief const accessor for the internally stored robosub command
    const RoboSubCommand& Command() const;

    // Command
    // \brief mutable accessor for the internally stored robosub command
    RoboSubCommand& Command();

    // SetThrustAll
    // \brief sets the total thrust for X,Y, Depth and Heading
    void SetThrustAll( const ThrustMode& modeX,
                       const ThrustMode& modeY,
                       const int32_t modeDepth,
                       const ThrustMode& modeHeading );

    // SetPneumAll
    // \brief sets the mode for all pneumatics devices
    void SetPneumAll( const PneumMode& modeTorpedo1,
                      const PneumMode& modeTorpedo2,
                      const PneumMode& modeMarker1,
                      const PneumMode& modeMarker2,
                      const PneumMode& modeClaw     );

    // SetThrustX
    // \brief sets the thrust for the x-axis (RightStrafe,LeftStrafe)
    void SetThrustX( const ThrustMode& mode );

    // SetThrustY
    // \brief sets the thrust for the y-axis (Forward/Backward)
    void SetThrustY( const ThrustMode& mode );
     
    // SetThrustDepth
    // \brief sets the thrust for the z-axis (Upward/Downward)
    void SetThrustDepth( const int32_t mode );

    // SetThrustHeading
    // \brief sets the thrust for turning. (Right/Left)
    void SetThrustHeading( const ThrustMode& mode );

    // SetPneumTorpedo1
    // \brief sets the fire mode for torpedo 1
    void SetPneumTorpedo1( const PneumMode& mode );

    // SetPneumTorpedo2
    // \brief sets the fire mode for torpedo 2
    void SetPneumTorpedo2( const PneumMode& mode );

    // SetPneumMarker1
    // \brief sets the fire mode for marker 1
    void SetPneumMarker1( const PneumMode& mode );

    // SetPneumMarker2
    // \brief sets the fire mode for marker 2
    void SetPneumMarker2( const PneumMode& mode );

    // SetPneumClaw
    // \brief sets the mode of the claw
    void SetPneumClaw( const PneumMode& mode );   

private:

    // _DataAvailableHandler
    // \brief callback for reading measurement data
    void _DataAvailableHandler( const boost::system::error_code& ec,
                                           size_t bytes_transferred );

    // _SentDataHandler
    // \brief callback for when data has been sent
    void _SentDataHandler( const boost::system::error_code& ec,
                           size_t bytes_transferred );
        
    // _SetThrustHelper
    // \brief helper function for setting the thrust
    void _SetThrustHelper( float& thruster, const ThrustMode& mode );
    
    // _SetPneumHelper
    // \brief helper function for setting the pneumatics devices
    void _SetPneumHelper( bool& pneum, const PneumMode& mode );

    // _ResetTimedPneumatics
    // \brief helper to reset torpedo and marker values after
    //        a command has been issued
    void _ResetTimedPneumatics();

private:
    static const float _NORMAL_THRUST_PWR;
    static const float _FULL_THRUST_PWR;

    static const bool _DEFAULT_PNEUMATIC_STATE;
    static const bool _ACTIVATED_PNEUMATIC_STATE;    
    
    boost::asio::io_service       _Io;
    boost::asio::io_service::work _Work;
    boost::asio::serial_port      _ArduinoPort;

    RoboSubCommand                _Command;
    ArduinoData                   _Status;
    
    char                          _SendBuffer[RoboSubControlData::SIZE];
    char                          _RecvBuffer[ArduinoData::SIZE];

    void (*_DataRecvCallback)(const ArduinoData&);
    void (*_DataSentCallback)(const RoboSubCommand&);
};

//////////////////////////////////////////////
// Since we can't use enum classes ( C++11 )
// with the current gcc, here's an attempt 
// at some scoped, type-safety, pseudo enums
// classes
//////////////////////////////////////////////

// Thrust Mode
// \brief determines the thrust mode
// usage: ThrustMode::POS_NORMAL
class ThrustMode 
{
public:

    static const unsigned int STOP       = 0;  // All Stop
    static const unsigned int POS_NORMAL = 1;  // Normal speed, positive thrust
    static const unsigned int POS_FULL   = 2;  // Full   speed, positive thrust
    static const unsigned int NEG_NORMAL = 3;  // Normal speed, negative thrust
    static const unsigned int NEG_FULL   = 4;  // Full   speed, negative thrust

    ThrustMode(unsigned int val ) 
        :
        _Val(STOP)
    { 
        _Val = ( (val == STOP)       || 
                 (val == POS_NORMAL) || 
                 (val == POS_FULL)   ||
                 (val == NEG_NORMAL) ||
                 (val == NEG_FULL)     ) ? val : STOP;
    }
    unsigned int& operator()() { return _Val; }
    const unsigned int& operator()() const { return _Val; }

private:
    unsigned int _Val;
};

// PneumMode
// \brief determines the pneumatics mode
// usage: PneumMode::ACTIVATED
class PneumMode 
{
public:

    static const unsigned int DEACTIVATED = 0;
    static const unsigned int ACTIVATED   = 1;

    PneumMode(unsigned int val)
        :
        _Val(DEACTIVATED)
    {
        _Val = ( (val == DEACTIVATED) ||
                 (val == ACTIVATED) ) ? val : DEACTIVATED;
    }
    unsigned int& operator()() { return _Val; }
    const unsigned int& operator()() const { return _Val; }

private:
    unsigned int _Val;
};

#endif //__ROBOSUB_CONTROLLER_H__
