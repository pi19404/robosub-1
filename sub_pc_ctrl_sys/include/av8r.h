#ifndef __AV8R_H__
#define __AV8R_H__

#include "real_joystick_handler.h"

#include <string>
#include <iostream>
#include <string>

// AV8R Events:
namespace Av8rEvents { 

enum AXIS {
     FIRST_AXIS         = 0
	,X					= FIRST_AXIS
	,Y
	,LEFT_THROTTLE
	,Z
	,RIGHT_THROTTLE
	,HAT_LR
	,HAT_UD
	,NUM_AXES
};

inline std::ostream& operator<<( std::ostream& os, const AXIS &ax )
{
    static std::string axis_strings[NUM_AXES] = 
            { "X", "Y", "LEFT_THROTTLE", "Z", "RIGHT_THROTTLE",
              "HAT_LR", "HAT_UD" };
    os << axis_strings[ax];

    return os;
}

enum BUTTON {
     FIRST_BUTTON       = 0
	,TRIGGER			= FIRST_BUTTON
	,FIRE
	,CENTER
	,RIGHT
	,T1
	,T2
	,T3
	,T4
	,T5
	,T6
	,T7
	,T8
	,MODE_A
	,MODE_B
	,NUM_BUTTONS
};
inline std::ostream& operator<<( std::ostream& os, const BUTTON &bu )
{
    static std::string btn_strings[NUM_BUTTONS] =
            { "TRIGGER", "FIRE", "CENTER", "RIGHT", "T1", "T2", "T3",
              "T4", "T5", "T6", "T7", "T8", "MODE_A", "MODE_B" };

    os << btn_strings[bu];

    return os;
}

}

// Av8r Joystick Handler Class
class Av8r 
    : public RealJoystickHandler
{

public:
	 int  Axes   [Av8rEvents::NUM_AXES];
	 bool Buttons[Av8rEvents::NUM_BUTTONS];

public:
	// Default Constructor
	Av8r();

	// Destructor.
	~Av8r();

	// Initialize Joystick
	// Args: std::string& -> device name
	// throws runtime exception for invalid device
    using RealJoystickHandler::Init;

	// Update
	// Performs a non-blocking read of the
	// joystick and updates the state variables.
	// returns number of events
    using RealJoystickHandler::Update;
    
private:
    Listener _ButtonListener;
    Listener _AxisListener;
};

#endif //__AV8R_H__
