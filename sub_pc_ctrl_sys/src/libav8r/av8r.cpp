#include "av8r.h"

#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdexcept>
#include <iostream>

using std::string;
using std::runtime_error;
using std::cerr;

void Av8r_ButtonCallback(void*, unsigned int, int);
void Av8r_AxisCallback(void*, unsigned int, int);

Av8r::Av8r()
	:
    RealJoystickHandler()
    ,Axes()
	,Buttons()
    ,_ButtonListener(static_cast<void*>(this), &Av8r_ButtonCallback)
    ,_AxisListener(static_cast<void*>(this), &Av8r_AxisCallback)
{ 
    // Initialize Data Structures
	for( int i = 0; i < Av8rEvents::NUM_AXES; ++i ) {
		Axes[i] = 0;
	}
	for( int i = 0; i < Av8rEvents::NUM_BUTTONS; ++i ) {
		Buttons[i] = false;
	}

    // Attach listeners to parent class
    RealJoystickHandler::AttachButtonListener(_ButtonListener);
    RealJoystickHandler::AttachAxisListener(_AxisListener);
}

Av8r::~Av8r() 
{

}

void Av8r_ButtonCallback(void* av8rObj, unsigned int btnId, int value)
{
    if( !av8rObj )
    {
        return;
    }

    namespace ae = Av8rEvents;

    if( btnId < ae::NUM_BUTTONS )
    {
        Av8r *target = static_cast<Av8r*>(av8rObj);
        target->Buttons[ btnId ] = value;
    }
}

void Av8r_AxisCallback(void* av8rObj, unsigned int axisId, int value)
{
    if( !av8rObj )
    {
        return;
    }
    
    namespace ae = Av8rEvents;

    if( axisId < ae::NUM_AXES )
    {
        Av8r *target = static_cast<Av8r*>(av8rObj);
        target->Axes[ axisId ] = value;
    }
}
