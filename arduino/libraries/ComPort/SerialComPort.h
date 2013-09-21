#ifndef __SERIAL_COM_PORT_H__
#define __SERIAL_COM_PORT_H__

#include <ComPort/ComPort.h>
#include <HardwareSerial.h>
#include <Logging/LogManager.h>

// ComPort Definition for Serial Devices
class SerialComPort 
	: 
	public ComPort
{
	public:
		SerialComPort( HardwareSerial& wire, uint8_t device, unsigned long timeOutMillis = 1500 );
		~SerialComPort();

		/* write
		* -description: write multiple bytes to a port
		* -args: uint8_t* bytes to write
		* -args: uint8_t number of bytes to write
		* -returns: none
		*/
		virtual void    write(uint8_t*, uint8_t);

		/* read
		* -description: read multiple bytes from a port
		* -args: uint8_t* storage for read bytes
		* -args: uint8_t number of bytes to read
		* -returns: none
		*/
		virtual void    read (uint8_t*, uint8_t);

		/* read
		* -description: read from a port where the
		* -description: initiation of a read requires args
		* -args: uint8_t* write args
		* -args: uint8_t number of write args
		* -args: uint8_t* read args
		* -args: uint8_t number of read args
		* -returns: none
		*/
		virtual void read(uint8_t*, uint8_t, uint8_t*, uint8_t);

		/* getDeviceAddress
		* -description: return the address of the device
		* -returns: device address
		*/
		uint8_t getDeviceAddress( void ){ return mDevice; }

	private:
		SerialComPort(const SerialComPort&);
		const SerialComPort& operator=(const SerialComPort&);

	private: 
		HardwareSerial& mSerial;
		unsigned long   mTimeOut;
};

#include "SerialComPort.cpp"

#endif //__SERIAL_COM_PORT_H__
