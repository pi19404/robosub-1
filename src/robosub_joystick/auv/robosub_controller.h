#ifndef __ROBOSUB_CONTROLLER_H__
#define __ROBOSUB_CONTROLLER_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_contorller.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication 
// Description: Class spec for RoboSubController, which reads commands from a 
//              input file (most likely a pipe from the AI or JoystickPublisher)
//              and writes control system messages to a the arduino serial port.
//
// Major Changes:
// 01-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_command.h"        // Command Data 

#include <boost/asio.hpp>

#include <fstream>
#include <string>

// RoboSubController
// \brief Converts command messages into control messages.
class RoboSubController
{
public:
    
    // Constructor
    RoboSubController( std::string infile, 
                       std::string comPort,
                       std::string baudRate );

    // Destructor
    ~RoboSubController();

    // Run
    // \brief runs the controller program
    void Run();

private:

    std::ifstream               _InputStream;
    boost::asio::io_service     _Io;
    boost::asio::serial_port    _ArduinoPort;
};


#endif //__ROBOSUB_CONTROLLER_H__
