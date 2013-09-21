#ifndef __CONFIG_H__
#define __CONFIG_H__
///////////////////////////////////////////////////////////////////////////////
// \file config.h
///////////////////////////////////////////////////////////////////////////////
// Author:
// Project: RoboSub Joystick Communication
// Description: Defines configuration parameters for the RoboSub joystick 
//              publisher.
//
// Major Changes:
// 26-May-2013      JS      Created File.
///////////////////////////////////////////////////////////////////////////////
#include "joystick_adapter_interface.h"

/*
 * Adapter Function Objects
 */

// AxisToThrusterBase
// \brief base class for all axis to thruster conversions
typedef AdapterFunctor<int,float> AxisToThrusterBase;

// AxisToThrusterX
// \brief converts joystick axis value to thruster X power
class AxisToThrusterX : public AxisToThrusterBase
{
    float operator()(int val)
    {
        // joystick value is -/+ 32767
        // required value is -/+ 100.0
        float res = val * 100.0;
              res /= 32767.0;

        return res;
    }
};


// AxisToThrusterY
// \brief converts joystick axis value to thruster Y power
class AxisToThrusterY : public AxisToThrusterBase
{
    float operator()(int val)
    {
        // joystick value is -/+ 32767
        // required value is +/- 100.0
        float res = val * 100.0;
              res /= -32767.0;

        return res;
    }
};


// AxisToThrusterHeading
// \brief converts joystick axis value to thruster heading power
typedef AxisToThrusterX AxisToThrusterHeading;


// AxisToThrusterDepth
// \brief converts joystick axis value to thruster depth power
class AxisToThrusterDepth : public AxisToThrusterBase
{
    float operator()(int val)
    {
        // joystick value is [-32767, 32767]  
        // required value is [0, -100]
        float res = val + 32767.0;
              res *= -100.0;
              res /= (2 * 32767.0);

        return res;
    }
};

// ButtonToPneumaticBase
// \brief base class for all button to pneumatic conversion
typedef AdapterFunctor<int,bool> ButtonToPneumaticBase;

// ButtonToPneumatic
// \brief converts a button value to pneumatic value
class ButtonToPneumatic : public ButtonToPneumaticBase
{
    bool operator()(int val)
    {
        // joystick value is 0 or 1
        // required value is false or true
        return static_cast<bool>(val);
    }
};


#endif //__CONFIG_H__
