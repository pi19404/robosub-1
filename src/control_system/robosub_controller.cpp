///////////////////////////////////////////////////////////////////////////////
// \file robosub_contorller.cpp
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Control System
// Description: Class impl for RoboSubController, which provides the Control
//                  System API to the AI. 
//
// Major Changes:
// 09-Jul-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_controller.h"
#include "robosub_control_data.h"
#include "arduino_data.h"

#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sstream>

using std::istream;
using std::ostringstream;
using std::exception;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using boost::asio::io_service;
using boost::asio::serial_port;
using boost::asio::buffer;

const float RoboSubController::_NORMAL_THRUST_PWR = 50.0;
const float RoboSubController::_FULL_THRUST_PWR    = 80.0;

const bool RoboSubController::_DEFAULT_PNEUMATIC_STATE   = false;
const bool RoboSubController::_ACTIVATED_PNEUMATIC_STATE = true;

RoboSubController::RoboSubController( string comPort, string baudRate )
    :
    _Io(),
    _ArduinoPort(_Io),
    _Command(),
    _Status(),
    _SendBuffer(NULL),
    _RecvBuffer(NULL)
{

    // Open Arduino Communications Port:
    try
    {
        namespace ba = boost::asio;
        using ba::serial_port_base;
        typedef ba::serial_port_base spb;

        // Open the port
        cout << "CONTROL_SYS>> Initializing Com Port" << endl;
        _ArduinoPort.open( comPort );
        _ArduinoPort.set_option( spb::baud_rate( atoi( baudRate.c_str() ) ) );

        // Wait for the arduino to initialize:
        cout << "CONTROL_SYS>> Waiting For Arduino To Initialize..." << endl;
        usleep(2000000);

        _SendBuffer = new char[RoboSubControlData::SIZE];
        _RecvBuffer = new char[ArduinoData::SIZE];
        
    }
    catch(...)
    {
        throw std::runtime_error("Failed to open port: " + comPort);
    }
}

RoboSubController::~RoboSubController()
{
    // close the arduino port
    _ArduinoPort.close();

    // clear the buffers
    if( _SendBuffer )
    {
        delete[] _SendBuffer;
        _SendBuffer = NULL;
    }

    if( _RecvBuffer )
    {
        delete[] _RecvBuffer;
        _RecvBuffer = NULL;
    }
}

void RoboSubController::SendCommand(ArduinoStatus& response)
{
    // Data Transfer Loop
    RoboSubControlData controlData;
    ArduinoData        arduinoData;
    
    // Convert Command to Control Message
    controlData.FromCommand( &_Command );

    // Send to Arduino
    namespace ba = boost::asio;
    controlData.SerializeToString( _SendBuffer );
    write( _ArduinoPort, ba::buffer( _SendBuffer, RoboSubControlData::SIZE ) ) ;

    // Get status from arduino
    read( _ArduinoPort, ba::buffer( _RecvBuffer, ArduinoData::SIZE ) );
    arduinoData.DeserializeFromString( _RecvBuffer );

    // Emit Response
    response = arduinoData;

    // Reset Timed Pneumatics
    _ResetTimedPneumatics();
}

const RoboSubCommand& RoboSubController::Command() const
{
    return _Command;
}

RoboSubCommand& RoboSubController::Command()
{
    return _Command;
}

void RoboSubController::SetThrustAll( const ThrustMode& modeX,
                                      const ThrustMode& modeY,
                                      const ThrustMode& modeDepth,
                                      const ThrustMode& modeHeading )
{
    SetThrustX(modeX);
    SetThrustY(modeY);
    SetThrustDepth(modeDepth);
    SetThrustHeading(modeHeading);
}

void RoboSubController::SetPneumAll( const PneumMode& modeTorpedo1,
                                     const PneumMode& modeTorpedo2,
                                     const PneumMode& modeMarker1,
                                     const PneumMode& modeMarker2,
                                     const PneumMode& modeClaw     )
{
    SetPneumTorpedo1(modeTorpedo1);
    SetPneumTorpedo2(modeTorpedo2);
    SetPneumMarker1(modeMarker1);
    SetPneumMarker2(modeMarker2);
    SetPneumClaw(modeClaw);
}

void RoboSubController::SetThrustX( const ThrustMode& mode )
{
    _SetThrustHelper( _Command.PowerX, mode );
}

void RoboSubController::SetThrustY( const ThrustMode& mode )
{
    _SetThrustHelper( _Command.PowerY, mode );
}
 
void RoboSubController::SetThrustDepth( const ThrustMode& mode )
{
    _SetThrustHelper( _Command.Depth, mode );
}

void RoboSubController::SetThrustHeading( const ThrustMode& mode )
{
    _SetThrustHelper( _Command.Heading, mode );
}

void RoboSubController::SetPneumTorpedo1( const PneumMode& mode )
{
    _SetPneumHelper( _Command.Torpedo1_Fire, mode );
}

void RoboSubController::SetPneumTorpedo2( const PneumMode& mode )
{
    _SetPneumHelper( _Command.Torpedo2_Fire, mode );
}

void RoboSubController::SetPneumMarker1( const PneumMode& mode )
{
    _SetPneumHelper( _Command.Marker1_Drop, mode );
}

void RoboSubController::SetPneumMarker2( const PneumMode& mode )
{
    _SetPneumHelper( _Command.Marker2_Drop, mode );
}

void RoboSubController::SetPneumClaw( const PneumMode& mode )
{
    _SetPneumHelper( _Command.Claw_Latch, mode );
}
 
void RoboSubController::_SetThrustHelper( float& thruster, 
                                          const ThrustMode& mode )
{
    switch ( mode() )
    {
        case ThrustMode::STOP:
            thruster = 0.0;
            break;
        case ThrustMode::POS_NORMAL:
            thruster = _NORMAL_THRUST_PWR;
            break;
        case ThrustMode::NEG_NORMAL:
            thruster = _NORMAL_THRUST_PWR * -1.0;
            break;
        case ThrustMode::POS_FULL:
            thruster = _FULL_THRUST_PWR;
            break;
        case ThrustMode::NEG_FULL:
            thruster = _FULL_THRUST_PWR * -1.0;
            break;
        default:
            thruster = 0.0;
    }
}

void RoboSubController::_SetPneumHelper( bool& pneum,
                                         const PneumMode& mode )
{
    switch( mode() )
    {
        case PneumMode::DEFAULT:
            pneum = _DEFAULT_PNEUMATIC_STATE;
            break;
        case PneumMode::ACTIVATED:
            pneum = _ACTIVATED_PNEUMATIC_STATE;
            break;
        default:
            pneum = _DEFAULT_PNEUMATIC_STATE; 
    }
}

void RoboSubController::_ResetTimedPneumatics()
{
    _Command.Torpedo1_Fire = _DEFAULT_PNEUMATIC_STATE;
    _Command.Torpedo2_Fire = _DEFAULT_PNEUMATIC_STATE;
    _Command.Marker1_Drop  = _DEFAULT_PNEUMATIC_STATE;
    _Command.Marker2_Drop  = _DEFAULT_PNEUMATIC_STATE;
}
