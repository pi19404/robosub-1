///////////////////////////////////////////////////////////////////////////////
// \file main.cpp
///////////////////////////////////////////////////////////////////////////////
// Author:      Jay Sidhu (JS) <jaypal.sidhu@gmail.com>
// Project:     RoboSub  Communication
// Description: Main program file for reading commands, converting them
//              to control messages and sending them to the arduino
//
// Major Changes:
// 1-Jun-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_controller.h" // reads from file, sends command to arduino.
#include "arduino_data.h"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using std::cout;
using std::cout;
using std::cerr;
using std::endl;
using std::fstream;
using std::istream;
using std::string;
using std::exception;

int main( int argc, char **argv )
{
    string serial_dev;
    string baud_rate;
    string infile_name;
    
   /*
    * Parse the command line
    */
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
       ("help", "Show this help message.")
       ("serial_port,s", po::value<std::string>(&serial_dev)->required()
                       , "arduino serial port.")
       ("baud_rate,b", po::value<std::string>(&baud_rate)->required()
                     , "serial port baud rate.");


    po::positional_options_description posOptions;
    posOptions.add("infile", 1);

    po::variables_map vm;

    try 
    {
        po::store(po::command_line_parser(argc, argv).options(desc)
                        .positional(posOptions).run(), vm);

        if( vm.count("help") )
        {
            cout << desc << endl;
            return 0;
        }

        po::notify(vm);
    }
    catch(exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        cout << desc << endl;
        return 1;
    }

   /*
    * Run the Controller. 
    */
    RoboSubController Controller(serial_dev, baud_rate);
    ArduinoData arduinoReply;

    Controller.SetThrustAll( ThrustMode::STOP,
                             ThrustMode::POS_FULL,
                             ThrustMode::NEG_FULL,
                             ThrustMode::STOP );

    Controller.SetPneumAll( PneumMode::ACTIVATED,
                            PneumMode::DEFAULT,
                            PneumMode::DEFAULT,
                            PneumMode::ACTIVATED,
                            PneumMode::ACTIVATED );
    
    cout << "Command Sent to Aruino: " << endl;
    cout << Controller.Command() << endl;

    Controller.SendCommand(arduinoReply);

    cout << "Status Received from Arduino: " << endl;
    cout << arduinoReply << endl;

    return 0;
}


