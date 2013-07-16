///////////////////////////////////////////////////////////////////////////////
// \file robosub_contorller.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication 
// Description: Class impl for RoboSubController, which reads commands from a 
//              input file (most likely a pipe from the AI or JoystickPublisher)
//              and writes control system messages to a the arduino serial port.
//
// Major Changes:
// 01-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_controller.h"

#include <exception>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <sstream>

#include <boost/bind.hpp>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

RoboSubController::RoboSubController()
    :
    _InputStream(),
    _Io(),
    _Work(_Io),
    _ArduinoPort(_Io),
    _SendQueueCounter(0),
    _Command(),
    _ControlData(),
    _CommandBuffer(),
    _SendBuffer(),
    _RecvBuffer()
{
    size_t i;
    for(i=0; i < sizeof(RoboSubCommand); ++i )
    {
        _CommandBuffer[i] = 0;
    }

    for(i=0; i < RoboSubControlData::SIZE; ++i)
    {
        _SendBuffer[i] = 0;
    }

    for(i=0; i < ArduinoData::SIZE; ++i)
    {
        _RecvBuffer[i] = 0;
    }
}

RoboSubController::~RoboSubController()
{

}

void RoboSubController::Run( string infile, 
                             string comPort, string baudRate )
{
    // Open infile
    try
    {
        _InputStream.open(infile.c_str(), std::ios::binary);
    }
    catch (std::exception& e)
    {
        cerr << e.what() << endl;
        throw std::runtime_error(e.what());
    }

    // Open Arduino Communications Port:
    try
    {
        namespace ba = boost::asio;
        using ba::serial_port_base;
        typedef ba::serial_port_base spb;

        // Open the port
        cout << "Initializing Com Port" << endl;
        _ArduinoPort.open( comPort );
        _ArduinoPort.set_option( spb::baud_rate( atoi( baudRate.c_str() ) ) );

        // Wait for the arduino to initialize:
        cout << "Waiting For Arduino..." << endl;
        usleep(2000000);

        // Enable Receiving of Arduino Data
        _DataAvailableHandler( boost::system::error_code(), 0 );
    }
    catch(...)
    {
        throw std::runtime_error("Error: Failed to open port: " + comPort);
    }

    // Data Transfer Loop
    while(_InputStream.good())
    {
        // Get the next command
        _GetNextCommand();
       
        // Send Control Data from Command
        _SendControlData();
     }
}

void RoboSubController::_GetNextCommand()
{
    // Read Command message from input stream
    _InputStream.read(_CommandBuffer, sizeof(RoboSubCommand));
    string buffStr(_CommandBuffer, sizeof(RoboSubCommand));

    // Deserialize the command
    _Command.DeserializeFromString(buffStr);

    // Print Received Command
    cout << "\nReceived Command: " << endl;
    cout << _Command << endl;
}

void RoboSubController::_SendControlData()
{
    namespace ba = boost::asio;

    // Send Control Data if queue is not full
    if( _SendQueueCounter < _SEND_QUEUE_MAX_SIZE )
    {
        // Convert Command to Control Data
        _ControlData.FromCommand( &_Command );

        // Serialize and send
        _ControlData.SerializeToString( _SendBuffer );
        ba::async_write( _ArduinoPort,
                        ba::buffer(_SendBuffer,RoboSubControlData::SIZE),
                        ba::transfer_at_least(RoboSubControlData::SIZE),
                        boost::bind( 
                            &RoboSubController::_SentDataHandler, 
                            this, 
                            ba::placeholders::error,
                            ba::placeholders::bytes_transferred ) );

        // Increment queue counter and initiate transfer
        ++_SendQueueCounter;
        _Io.poll();
    }
}

void RoboSubController::_DataAvailableHandler( 
                                           const boost::system::error_code& ec,
                                           size_t bytes_transferred )
{  
    namespace ba = boost::asio;

    // Case: All data is here, and checks out.
    if( ( bytes_transferred == ArduinoData::SIZE-1 ) &&
        ( _RecvBuffer[0] == ArduinoStatus::MAGIC ) )
    {

        if( ArduinoStatus::SerializedIsValid(_RecvBuffer, ArduinoData::SIZE) )
        {
            ArduinoData recvData;
            recvData.DeserializeFromString(_RecvBuffer);
            cout << "\nReceived Status: " << endl;
            cout << recvData << endl;
        }
    }
    // Case: first byte found
    else if( _RecvBuffer[0] == ArduinoStatus::MAGIC )
    {   
        ba::async_read ( _ArduinoPort,
                         ba::buffer(&_RecvBuffer[1],ArduinoData::SIZE-1),
                         ba::transfer_at_least(ArduinoData::SIZE-1),
                         boost::bind( 
                             &RoboSubController::_DataAvailableHandler, 
                             this, 
                             ba::placeholders::error,
                             ba::placeholders::bytes_transferred ) );
        _Io.poll();
        return;
    } 
    ba::async_read ( _ArduinoPort,
                     ba::buffer(_RecvBuffer,1),
                     ba::transfer_at_least(1),
                     boost::bind( 
                         &RoboSubController::_DataAvailableHandler, 
                         this, 
                         ba::placeholders::error,
                         ba::placeholders::bytes_transferred ) );
    _Io.poll();
}

void RoboSubController::_SentDataHandler( 
                                          const boost::system::error_code& ec,
                                          size_t bytes_transferred )
{
    // Write info to stdout
    cout << "\nSent Data: " << endl;
    cout << _ControlData << endl;

    if( _SendQueueCounter )
    {
        --_SendQueueCounter;
    }
}


