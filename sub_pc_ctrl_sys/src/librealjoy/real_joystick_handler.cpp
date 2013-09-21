#include "real_joystick_handler.h"

#include <linux/joystick.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <stdexcept>
#include <iostream>

using std::string;
using std::runtime_error;
using std::cerr;

RealJoystickHandler::RealJoystickHandler()
	:
     JoystickHandlerInterface()
    ,_ButtonListeners()
    ,_AxisListeners()
	,_Jfd(-1)
{ }
     


RealJoystickHandler::~RealJoystickHandler() {

	if( _Jfd > 0 ) {
        close(_Jfd);
    }
}

void RealJoystickHandler::Init( string& deviceName ) {

	// Init should only be called once:
	if( _Jfd > -1 ){ return; }

	// try to open joystick port
	_Jfd = open(deviceName.c_str(), O_NONBLOCK | O_RDONLY);

	// throw error if unsucessful
	if( _Jfd < 0 ){ 
		throw runtime_error("Failed to open joystick: " + deviceName); 
	}
}
 
int RealJoystickHandler::Update(){

	if( _Jfd < 0 ){
		throw runtime_error("Failed to read from joystick.");
	}

	// read all events in queue and process
	int nEvents = 0;
	struct js_event jse;
	while( read(_Jfd, &jse, sizeof(struct js_event) ) > 0 ) {
		++nEvents;
		_ProcessEvent(&jse);
	}

	// check errno
	if( EAGAIN != errno ){

		// throw runtime_exception
		throw runtime_error("Joystick was disconnected.");
	}
	return nEvents;	
}

void RealJoystickHandler::_ProcessEvent(void* jsEvent ) {
	
	// Extract Event
	struct js_event* e = (struct js_event *)jsEvent;

	// process Event
	switch( e->type ) {
	
	case JS_EVENT_BUTTON:
			
        _NotifyButtonListeners( e->number, e->value );
		break;

	case JS_EVENT_AXIS:

		_NotifyAxisListeners( e->number, e->value );
        break;
	
	// Ignore Synthetic Events:
	case (JS_EVENT_BUTTON | JS_EVENT_INIT): ;
	case (JS_EVENT_AXIS   | JS_EVENT_INIT): ;
	default: ;;
	}
}


void RealJoystickHandler::_NotifyButtonListeners( unsigned int eventId,
                                                           int value    )
{
    std::vector<Listener>::iterator it;
    for( it  = _ButtonListeners.begin(); 
         it != _ButtonListeners.end(); ++it ){

        (*it)(eventId, value);
    }
}

void RealJoystickHandler::_NotifyAxisListeners( unsigned int eventId,
                                                           int value    )
{
    std::vector<Listener>::iterator it;
    for( it  = _AxisListeners.begin(); 
         it != _AxisListeners.end(); ++it ){

        (*it)(eventId, value);
    }
}
