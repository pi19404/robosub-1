#ifndef __ARDUINO_COM_MESSAGE_H__
#define __ARDUINO_COM_MESSAGE_H__
///////////////////////////////////////////////////////////////////////////////
/// ArduinoComMessage.h
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************
* Author: JSS (jaypal.sidhu {at} email {dot} wsu {dot} edu)
* Date: 1/26/12
* Project: RoboSub Senior Design Project
* Brief: Defines the Arduino extension of ComMessage -> ArduinoComMessage. 
* Description:  This class provides	a message communication protocol for the 
*				Arduino	embedded systems of the RoboSub project.  All systems, 
*				embedded and otherwise, will communicate based on interface 
*				defined by the (parent) ComMessage protocol. This implemtation 
*				leverages the String class of the Arduino environment, but 
*				provides the same interface as the ComMessage protocol.
*
* Revision History: | Date    | Changes										  |
*					| 1/27/12 | Added IsComMessage() prototype.				  |
*
******************************************************************************/
#if (ARDUINO >= 100)
    #include <Arduino.h>
    #define BUFFER buffer
#else
    #include <WProgram.h>
    #define BUFFER _buffer
#endif

#include "ComMessage.h"	//X-Platform RoboSub message communication protocol.

/*
 * ArduinoComMessage
 * Arduino version of the ComMessage Protocol.
 * In other words, it uses the Arduino String class.
 */
class ArduinoComMessage : public ComMessage<String>
{
  private:
        /*
         * MString - because M-theory extends String theory. ;-)
         * Designed to extend the Arduino String class.
         * implements a ToCString method (like cstring() of CPP Standard Library)
         * Do not use this class for anything else but the sole method.
         */
        class MString : public String
        {
          public:
            MString( const String& rhs ) 
                : 
                String( rhs ) 
            { 
            
            }
            char *ToCString( void ) { return BUFFER; }
        };
  public:
  
	/*
	 * Default Constructor.
	 */
	ArduinoComMessage( void )
			:
			ComMessage<String>()
			{
			 // Nothing left to do here.
			}

	/*
	 * Standard Constructor.
	 */
	ArduinoComMessage(  
						 const String& system
						,const String& subSystem
						,const String& messageType
						,const String& messageData 
					 )
		:
		ComMessage<String>( system, subSystem, messageType, messageData )
		{
			// Nothing left to do here.
		}

	/*
	 * Copy Constructor. (explicit: to save space on Arduino)
	 */
	explicit ArduinoComMessage( const ArduinoComMessage& comMessage )
		:
		ComMessage<String>( comMessage )
		{
			// Nothing left to do here.
		}

	/*
	 * C-String Constructor.
	 */
	explicit ArduinoComMessage( char* messageString )
		:
		ComMessage<String>( messageString )
		{
			// Nothing left to do here.
		}

	
	/*
	 * Arduino String Constructor.
	 */
    ArduinoComMessage( const String& comMessageString )
		: 
		// Convert to MString to get access to internal storage type
		ComMessage<String>( MString( comMessageString ).ToCString() )
		{
			//Nothing left to do.
		}
};

#endif //__ARDUINO_COM_MESSAGE_H__
