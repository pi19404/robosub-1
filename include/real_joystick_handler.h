#ifndef __REAL_JOYSTICK_HANDLER_H__
#define __REAL_JOYSTICK_HANDLER_H__

#include "joystick_handler_interface.h"   // Joystick interface class
#include <string>
#include <vector>

class RealJoystickHandler
    : public JoystickHandlerInterface
{
public:

	// Default Constructor
	RealJoystickHandler();

	// Destructor.
	virtual ~RealJoystickHandler();

	// Initialize Joystick
	// Args: std::string& -> device name
	// throws runtime exception for invalid device
	virtual void Init(std::string& );

	// Update
	// Performs a non-blocking read of the
	// joystick and updates the state variables.
	// returns number of events
	virtual int Update();

protected:
    // Listener
    class Listener
    {
     public:
        Listener(   void *args,
                    void (*callback)(void *, unsigned int, int) )
        :
        _Args(args),
        _Callback(callback)
        { }

        // Implement this operator in derived classes
        // eventId is the button or axis number
        // value is the numerical value of the button or axis
        void operator()( unsigned int eventId, int value ){
            (_Callback)(_Args, eventId, value);
        }

        private:
            void *_Args;
            void (* _Callback)(void *, unsigned int, int);
    };

    // Attach Button Listener
    // Allows a subclass to be notified of a joystick
    // button event
    void AttachButtonListener( RealJoystickHandler::Listener& listener )
    {
        _ButtonListeners.push_back(listener);
    }

    // Attach Axis Listener
    // Allows a subclass to be notified of a joystick
    // axis event
    void AttachAxisListener( RealJoystickHandler::Listener& listener )
    {
        _AxisListeners.push_back(listener);
    }

private:
	void _ProcessEvent(void*);
    void _NotifyButtonListeners(unsigned int, int);
    void _NotifyAxisListeners(unsigned int, int);

    std::vector<Listener> _ButtonListeners;
    std::vector<Listener> _AxisListeners;

	int  _Jfd;
};

#endif //__REAL_JOYSTICK_HANDLER_H__
