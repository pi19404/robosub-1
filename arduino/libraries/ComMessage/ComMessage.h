#ifndef __COM_MESSAGE_HPP__
#define __COM_MESSAGE_HPP__
///////////////////////////////////////////////////////////////////////////////
/// ComMessage.h
///////////////////////////////////////////////////////////////////////////////
/******************************************************************************
* Author: JSS (jaypal.sidhu {at} email {dot} wsu {dot} edu)
* Date: 1/26/12
* Project: RoboSub Senior Design Project
* Brief: Defines the RoboSub Communication Protcol.
* Description:  This class provides	a message communication protocol for the 
*				RoboSub project. This protcol is platform-independent.
*				Extensions pass the appropriate string class as a template 
*				parameter. This is required because some embedded platforms
*				(i.e. Arduino) do not support the C++ standard library.
******************************************************************************/

/******************************************************************************
* Protocol Defintions:
*******************************************************************************
* A Message follows the following format:
*
*  #[System]$[SubSubsystem]>[MessageType]@[MessageData]
*
* Where,
*	[System] 		- is the major system name.
*   [SubSystem] 	- is the subsystem name.
*   [MessageType]	- is the message type.
*	[MessageData]	- is the raw form of the appropriate data.
******************************************************************************/
#define COM_MESSAGE_SYSTEM_HEADER '#'
#define COM_MESSAGE_SUBSYSTEM_HEADER '$'
#define COM_MESSAGE_TYPE_HEADER '>'
#define COM_MESSAGE_DATA_HEADER '@'


// Class Declaration:
template <class StrClass>
class ComMessage
{
	public:

		// Default Constructor.
		ComMessage( void );

		// Standard Constructor.
		ComMessage(  
					 const StrClass& system
					,const StrClass& subSystem
					,const StrClass& messageType
					,const StrClass& messageData 
				  );

		// Copy Constructor.
		ComMessage( const ComMessage<StrClass>& comMessage );

		// C-String Constructor.
		explicit ComMessage( char* messageString );

		// Determines if the input string is a message string.
		static bool IsComMessageString( const char* msgString );

		// Convert to message string for sending.
		StrClass ToString( void );
		
		// Setters and Getters for Data Members. ??beware!  information hiding violated??
		void	 SetSystem( const StrClass& system );
		StrClass GetSystem( void ) const;

		void	 SetSubSystem( const StrClass& subSystem );
		StrClass GetSubSystem( void ) const;
		
		void	 SetMessageType( const StrClass& messageType );
		StrClass GetMessageType( void ) const;

		void	 SetMessageData( const StrClass& messageData );
		StrClass GetMessageData( void ) const;

		template <class DataType>
		static StrClass DataToString( const DataType& dataObj );

		template <class DataType>
		static DataType StringToData( const StrClass& dataStr );

	private:
		StrClass mSystem;		// Corresponds to [System]
		StrClass mSubSystem;	// Corresponds to [SubSystem]
		StrClass mMessageType;	// Corresponds to [MessageType]
		StrClass mMessageData;	// Corresponds to [MessageData]
};

#include "ComMessage.cpp"

#endif //__COM_MESSAGE_HPP__
