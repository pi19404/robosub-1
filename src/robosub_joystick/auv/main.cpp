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
       ("infile,i", po::value<std::string>(&infile_name)
                  , "Input file for receiving commands.")
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
    RoboSubController Controller(infile_name, serial_dev, baud_rate);
    Controller.Run();

    return 0;
}


