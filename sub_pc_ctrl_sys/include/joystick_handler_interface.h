#ifndef __JOYSTICK_HANDLER_INTERFACE_H__
#define __JOYSTICK_HANDLER_INTERFACE_H__

#include <string>

// Generic Joystick Interface Class
// for use with linux joystick communication
class JoystickHandlerInterface 
{
public:
	// Default Constructor
	JoystickHandlerInterface(){ }

	// Destructor.
	virtual ~JoystickHandlerInterface(){ }

	// Initialize Joystick
	// Args: std::string& -> device name
	virtual void Init(std::string&) = 0;

	// Update
	// Reads the Joystick state.
	// returns number of events that have triggered
    virtual int Update() = 0;
};

#endif //__JOYSTICK_HANDLER_INTERFACE_H__
