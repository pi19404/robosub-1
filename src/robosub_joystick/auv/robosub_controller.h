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
#include "robosub_control_data.h"
#include "arduino_data.h"

#include <boost/asio.hpp>
#include <boost/signals2/mutex.hpp>

#include <fstream>
#include <string>

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
    // \brief runs the controller program
    // \throws runtime_error if any errors occur
    void Run( std::string infile, 
              std::string comPort,
              std::string baudRate );

private:

    // _GetNextCommand
    // \brief helper for getting the next command from the input file
    void _GetNextCommand();

    // _SendControlData
    // \brief helper for sending the derived control data from the command
    void _SendControlData();

    // _DataAvailableHandler
    // \brief callback for reading measurement data
    void _DataAvailableHandler( const boost::system::error_code& ec,
                                           size_t bytes_transferred );

    // _SentDataHandler
    // \brief callback for when data has been sent
    void _SentDataHandler( const boost::system::error_code& ec,
                           size_t bytes_transferred );


private:

    static const unsigned int     _SEND_QUEUE_MAX_SIZE = 10;

    std::ifstream                 _InputStream;
    boost::asio::io_service       _Io;
    boost::asio::io_service::work _Work;
    boost::asio::serial_port      _ArduinoPort;

    unsigned int                  _SendQueueCounter;

    RoboSubCommand                _Command;
    RoboSubControlData            _ControlData;
    
    char                          _CommandBuffer[sizeof(RoboSubCommand)];
    char                          _SendBuffer[RoboSubControlData::SIZE];
    char                          _RecvBuffer[ArduinoData::SIZE];

};


#endif //__ROBOSUB_CONTROLLER_H__
