#ifndef __ROBOSUB_JOYSTICK_PUBLISHER_H__
#define __ROBOSUB_JOYSTICK_PUBLISHER_H__
///////////////////////////////////////////////////////////////////////////////
// \file robosub_joystick_publisher.h
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication 
// Description: Class spec for RoboSubJoystickPublisher, which reads commands 
//              from the av8r joystick, converts them to RoboSub commands and 
//              sends them out to a file (if it exists) to the RoboSub AUV PC.
//
// Major Changes:
// 26-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "config.h"          // Configurations for joystick mappings
#include "robosub_command.h" // Command Data 
#include "av8r.h"
#include "av8r_adapter.h"

#include <fstream>
#include <string>

// RoboSubJoystickPublisher
// \brief converts joystick input into robosub control system commands
//        and then publishes those commands to the specified file
class RoboSubJoystickPublisher
{
public:
    
    // Constructor
    RoboSubJoystickPublisher( Av8r& joystick, std::string& outfile );

    // Destructor
    ~RoboSubJoystickPublisher();

    // Run
    // \brief runs the publishing program
    void Run();

private:

    // Setup Mappings
    // \brief creates the mappings between the joystick
    //        and the command data
    void SetupMappings();

    // Update Joystick Data
    // \brief reads and adapts the latest joystick data
    void UpdateJoystickData();

    // Publish Joystick Data
    // \brief writes the adapted data out to the specified stream
    void PublishJoystickData();
    
    // Private members
    Av8r&              _Joystick;
    std::ofstream      _OutputStream;
    RoboSubCommand     _Command;
    Av8rAdapter<bool>  _PneumaticsAdapter;
    Av8rAdapter<float> _ThrusterAdapter;

    // Garbage collectors:
    std::vector<ButtonToPneumaticBase *> _AllocatedPneumaticsFuncs;
    std::vector<AxisToThrusterBase *>    _AllocatedThrusterFuncs;
};


#endif //__ROBOSUB_JOYSTICK_PUBLISHER_H__
