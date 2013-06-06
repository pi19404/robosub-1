///////////////////////////////////////////////////////////////////////////////
// \file robosub_joystick_publisher.cpp
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication 
// Description: Class impl for RoboSubJoystickPublisher, which reads commands 
//              from the av8r joystick, converts them to RoboSub commands and 
//              sends them out to a file (if it exists) to the RoboSub AUV PC.
//
// Major Changes:
// 26-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_joystick_publisher.h"

#include <iostream>
#include <exception>
#include <cassert>

RoboSubJoystickPublisher::RoboSubJoystickPublisher( Av8r& joystick, 
                                                    std::string& outfile )
    :
    _Joystick(joystick),
    _OutputStream(),
    _Command(),
    _PneumaticsAdapter(joystick),
    _ThrusterAdapter(joystick),
    _AllocatedPneumaticsFuncs(),
    _AllocatedThrusterFuncs()
{
    try
    {
        _OutputStream.open(outfile.c_str(), std::ios::binary);
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
        assert(false);
    }
    SetupMappings();
}

RoboSubJoystickPublisher::~RoboSubJoystickPublisher()
{ 
    // deallocate mappings
    typename std::vector<ButtonToPneumaticBase *>::iterator it;
    for( it = _AllocatedPneumaticsFuncs.begin();
         it != _AllocatedPneumaticsFuncs.end(); ++it )
    {
        delete (*it);
    }

    typename std::vector<AxisToThrusterBase *>::iterator it2;
    for( it2 = _AllocatedThrusterFuncs.begin();
         it2 != _AllocatedThrusterFuncs.end(); ++it2 )
    {
        delete (*it2);
    }
}

void RoboSubJoystickPublisher::Run()
{
    // Do initial publish
    UpdateJoystickData();
    PublishJoystickData();

    while(true)
    {
        RoboSubCommand old = _Command;
        UpdateJoystickData();

        // Only update if we need to:
        if( _Command != old )
        {
            PublishJoystickData();
        }
    }
}

/*****************************************************************************
*
* BEGIN MAPPINGS CONFIGURATION: use this method to configure your mappings.
*
*****************************************************************************/
void RoboSubJoystickPublisher::SetupMappings()
{
    namespace ae = Av8rEvents;

    // Button Mappings:
    ButtonToPneumaticBase* pneuF = new ButtonToPneumatic;
    _AllocatedPneumaticsFuncs.push_back(pneuF);
    _PneumaticsAdapter.MapToButton( ae::T1, _Command.Torpedo1_Fire, *pneuF );
    _PneumaticsAdapter.MapToButton( ae::T2, _Command.Torpedo2_Fire, *pneuF );
    _PneumaticsAdapter.MapToButton( ae::T3, _Command.Marker1_Drop,  *pneuF );
    _PneumaticsAdapter.MapToButton( ae::T4, _Command.Marker2_Drop,  *pneuF );
    _PneumaticsAdapter.MapToButton( ae::MODE_A,_Command.Claw_Latch, *pneuF );

    // Axis Mappings:
    /* allocate handlers */
    AxisToThrusterBase* thrFX = new AxisToThrusterX;
    AxisToThrusterBase* thrFY = new AxisToThrusterY;
    AxisToThrusterBase* thrFH = new AxisToThrusterHeading;
    AxisToThrusterBase* thrFD = new AxisToThrusterDepth;
    
    /* add to allocated garbage can */
    _AllocatedThrusterFuncs.push_back(thrFX);
    _AllocatedThrusterFuncs.push_back(thrFY);
    _AllocatedThrusterFuncs.push_back(thrFH);
    _AllocatedThrusterFuncs.push_back(thrFD);
    
    /* do the mappings */
    _ThrusterAdapter.MapToAxis( ae::X, _Command.PowerX, *thrFX );
    _ThrusterAdapter.MapToAxis( ae::Y, _Command.PowerY, *thrFY );
    _ThrusterAdapter.MapToAxis( ae::Z, _Command.Heading,*thrFH );
    _ThrusterAdapter.MapToAxis( ae::LEFT_THROTTLE, _Command.Depth, *thrFD );
}
/*****************************************************************************
*
* END MAPPINGS CONFIGURATION
*
*****************************************************************************/

void RoboSubJoystickPublisher::UpdateJoystickData()
{
    _PneumaticsAdapter.Update();
    _ThrusterAdapter.Update();
}

void RoboSubJoystickPublisher::PublishJoystickData()
{
    std::string ostr;
    _Command.SerializeToString(ostr);

    // Write message to output file
    if( _OutputStream.good() )
    {
        _OutputStream << ostr;
        _OutputStream.flush();
    }
    
    // Write info to stdout
    std::cout << "\nSending Data:" << std::endl;
    std::cout << _Command;
}
