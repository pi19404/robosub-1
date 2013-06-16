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
#include "robosub_control_data.h"
#include "arduino_data.h"

#include <exception>
#include <cstdlib>
#include <cassert>
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

RoboSubController::RoboSubController( string infile, 
                                      string comPort, string baudRate )
    :
    _InputStream(),
    _Io(),
    _ArduinoPort(_Io)
{

    // Open infile
    try
    {
        _InputStream.open(infile.c_str(), std::ios::binary);
    }
    catch (exception& e)
    {
        cerr << e.what() << endl;
        assert(false);
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
       // _ArduinoPort.set_option( spb::flow_control( spb::flow_control::none ) );
       // _ArduinoPort.set_option( spb::stop_bits( spb::stop_bits::one ) );

        // Wait for the arduino to initialize:
        cout << "Waiting For Arduino..." << endl;
        usleep(2000000);
    }
    catch(...)
    {
        cerr << "Error: Failed to open port: " << comPort << endl;
        assert(false);
    }
}

RoboSubController::~RoboSubController()
{
    // close input stream
    _ArduinoPort.close();
}


void RoboSubController::Run()
{
    // Data Transfer Loop
    bool first_read = true;
    RoboSubCommand command;
    RoboSubControlData controlData;
    ArduinoData   arduinoData;
    char *sendBuffer = new char[RoboSubControlData::SIZE];
    char *recvBuffer = new char[ArduinoData::SIZE];
    while(_InputStream.good())
    {
        // Read Command message from input stream
        char buffer[sizeof(RoboSubCommand)];
        _InputStream.read(buffer, sizeof(RoboSubCommand));
        string buffStr(buffer, sizeof(RoboSubCommand));

        // discard the first read, joystick initial state
        if( first_read )
        {
            first_read = false;
        }
        else
        {
            command.DeserializeFromString(buffStr);
        }

        // Print Received Command
        cout << "\nReceived Data: " << endl;
        cout << command << endl;
        
        // Convert to Control Message
        controlData.FromCommand( &command );

        // Write info to stdout
        cout << "\nSending Data: " << endl;
        cout << controlData << endl;
         
        // Send to Arduino
        namespace ba = boost::asio;
        controlData.SerializeToString(sendBuffer);
        write( _ArduinoPort, ba::buffer( sendBuffer, RoboSubControlData::SIZE ) ) ;

        // Get status from arduino
        read( _ArduinoPort, ba::buffer( recvBuffer, ArduinoData::SIZE ) );
        arduinoData.DeserializeFromString(recvBuffer);

        // Write info to stdout
        cout << "\nReceived Status: " << endl;
        cout << arduinoData << endl;
    }
    delete [] sendBuffer;
    delete [] recvBuffer;
}
