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


///////////////////////////////////////////////////////////////////////////////
///
/// Class Declaration:
///
///////////////////////////////////////////////////////////////////////////////

template <class StrClass>
class ComMessage
{
public:

	/*
	 * Default Constructor.
	 */
	ComMessage( void );

	/*
	 * Standard Constructor.
	 */
	ComMessage(  
				 const StrClass& system
				,const StrClass& subSystem
				,const StrClass& messageType
				,const StrClass& messageData 
			  );

	/*
	 * Copy Constructor.
	 */
	ComMessage( const ComMessage<StrClass>& comMessage );

	/*
	 * C-String Constructor.
	 */
	explicit ComMessage( char* messageString );

	/*
	 * ToString.
	 *  Convert to message string for sending.
	 */
	StrClass ToString( void );
	
	/*
	 * IsComMessageString
	 *  Determines if the input string is a message string.
	 */
	static bool IsComMessageString( const char* msgString );
	
	/*
	 * Setters and Getters for Data Members.
	 */
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


///////////////////////////////////////////////////////////////////////////////
///
/// Template Specializations:
///
///////////////////////////////////////////////////////////////////////////////
template <class StrClass>
ComMessage<StrClass>::ComMessage( void )
	:
	 mSystem(StrClass())
	,mSubSystem(StrClass())
	,mMessageType(StrClass())
	,mMessageData(StrClass())
	 {
		 // Nothing left to do here.
	 }

template <class StrClass>
ComMessage<StrClass>::ComMessage(  
			 const StrClass& system
			,const StrClass& subSystem
			,const StrClass& messageType
			,const StrClass& messageData 
		  )
	:
	 mSystem(system)
	,mSubSystem(subSystem)
	,mMessageType(messageType)
	,mMessageData(messageData)
	{
		// Nothing left to do here.
	}

template <class StrClass>
ComMessage<StrClass>::ComMessage( const ComMessage<StrClass>& comMessage )
	:
	 mSystem( StrClass(comMessage.mSystem) )
	,mSubSystem( StrClass(comMessage.mSubSystem))
	,mMessageType( StrClass(comMessage.mMessageType))
	,mMessageData( StrClass(comMessage.mMessageData))
	 {
		 // Nothing left to do here.
	 }

template <class StrClass>
ComMessage<StrClass>::ComMessage( char* messageString )
{
	if( IsComMessageString( messageString ) )
	{
		char *substart
			,*subend;
				
		substart = messageString;
		++substart;
		subend = substart;
			
		while( *subend != COM_MESSAGE_SUBSYSTEM_HEADER ) { ++subend; }
		
		*subend = 0;
		mSystem = StrClass( substart );
		*subend = COM_MESSAGE_SUBSYSTEM_HEADER;
		
		++subend;
		substart = subend;
		while( *subend != COM_MESSAGE_TYPE_HEADER ) { ++subend; }
		
		*subend = 0;
		mSubSystem = StrClass( substart );
		*subend = COM_MESSAGE_TYPE_HEADER;
		
		++subend;
		substart = subend;
		while( *subend != COM_MESSAGE_DATA_HEADER ) { ++subend; }
		
		*subend = 0;
		mMessageType = StrClass( substart );
		*subend = COM_MESSAGE_DATA_HEADER;
		
		++subend;
		mMessageData = StrClass( subend );	
	}
}
template <class StrClass>
StrClass ComMessage<StrClass>::ToString( void )
{
	return COM_MESSAGE_SYSTEM_HEADER + \
		   mSystem + \
		   COM_MESSAGE_SUBSYSTEM_HEADER + \
		   mSubSystem + \
		   COM_MESSAGE_TYPE_HEADER + \
		   mMessageType + \
		   COM_MESSAGE_DATA_HEADER + \
		   mMessageData;
}

template <class StrClass>
bool ComMessage<StrClass>::IsComMessageString( const char* msgString )
 {
	if( msgString && (*msgString == COM_MESSAGE_SYSTEM_HEADER) )
	{
		++msgString;
		char header[3] = { 
							 COM_MESSAGE_SUBSYSTEM_HEADER
							,COM_MESSAGE_TYPE_HEADER
							,COM_MESSAGE_DATA_HEADER 
						 };
						 
		for( int i = 0; i < 3; ++i )
		{
			while( *msgString && (*msgString != header[i] ) )
			{
				++msgString;
			}
			
			if( !( *msgString ) )
			{
				return false;
			}
		}
		return true;
	}
	return false;
 }

template <class StrClass>
void	 ComMessage<StrClass>::SetSystem( const StrClass& system ) { mSystem = system; }

template <class StrClass>
StrClass ComMessage<StrClass>::GetSystem( void ) const { return mSystem; }

template <class StrClass>
void	 ComMessage<StrClass>::SetSubSystem( const StrClass& subSystem ) { mSubSystem = subSystem; }

template <class StrClass>
StrClass ComMessage<StrClass>::GetSubSystem( void ) const { return mSubSystem; }

template <class StrClass>
void	 ComMessage<StrClass>::SetMessageType( const StrClass& messageType ) { mMessageType = messageType; }

template <class StrClass>
StrClass ComMessage<StrClass>::GetMessageType( void ) const { return mMessageType; }

template <class StrClass>
void	 ComMessage<StrClass>::SetMessageData( const StrClass& messageData ) { mMessageData = messageData; }

template <class StrClass>
StrClass ComMessage<StrClass>::GetMessageData( void ) const { return mMessageData; }

template <class StrClass> template <class DataType>
StrClass ComMessage<StrClass>::DataToString( const DataType& dataObj )
{
	const char* dataCStr = reinterpret_cast<const char *>(  &const_cast<DataType&>( dataObj ) );
	StrClass dataStr;

	for( int i = 0; i < sizeof(DataType); ++i )
	{
		dataStr += dataCStr[i];
	}
	return dataStr;
}

template <class StrClass> template <class DataType>
DataType ComMessage<StrClass>::StringToData( const StrClass& dataStr )
{
	DataType data;
	char *dataCStr = reinterpret_cast<char *>( &data );

	for( int i = 0; i < sizeof(DataType); ++i )
	{
		dataCStr[i] = dataStr[i];
	}
	return data;
}

#endif //__COM_MESSAGE_HPP__