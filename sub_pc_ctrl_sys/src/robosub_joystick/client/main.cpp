///////////////////////////////////////////////////////////////////////////////
// \file main.cpp
///////////////////////////////////////////////////////////////////////////////
// Author:      Jay Sidhu (JS) <jaypal.sidhu@gmail.com>
// Project:     RoboSub Joystick Communication
// Description: Main program file for converting joystick commands and sending
//              them to the RoboSub AUV.
//
// Major Changes:
// 25-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_joystick_publisher.h" // reads from av8r, publishes to sub.
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <exception>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ostream;
using std::string;
using std::exception;

int main( int argc, char **argv )
{
    string joystick_dev;
    string outfile_name;

    Av8r joystick;
    
   /*
    * Parse the command line
    */
    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
       ("help", "Show this help message.")
       ("joystick,j", po::value<std::string>(&joystick_dev)->required()
                  , "Joystick file device.")
       ("outfile,o", po::value<std::string>(&outfile_name)
                 , "Output file for decoded commands");

    po::positional_options_description posOptions;
    posOptions.add("joystick", 1);

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

    try
    {
        joystick.Init(joystick_dev);
    }
    catch(exception& e)
    {
        cerr << "Error: " << e.what() << endl;
        return 2;
    }

   /*
    * Run the publisher
    */
    RoboSubJoystickPublisher Publisher(joystick,outfile_name);
    Publisher.Run();

    return 0;
}


