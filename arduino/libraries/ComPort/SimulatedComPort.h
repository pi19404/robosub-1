#ifndef __SIMULATED_COM_PORT_H__
#define __SIMULATED_COM_PORT_H__

#include <stdint.h>
#include <Containers/Queue.h>
#include <ComPort/ComPort.h>
#include <Device/SimulatedDevice.h>
#include <Device/SimulatedDeviceController.h>

class SimulatedComPort
	:
	public ComPort,
	public SimulatedDevice
{
	public:
		SimulatedComPort( uint8_t, SimulatedDeviceController& );

		~SimulatedComPort();

		///////////////////////////////////////////
		// ComPort Methods:
		///////////////////////////////////////////

		/* write
		* -description: write multiple bytes to a port
		* -args: uint8_t port address
		* -args: uint8_t* bytes to write
		* -args: uint8_t number of bytes to write
		* -returns: none
		*/
		virtual void    write(uint8_t*, uint8_t );

		/* read
		* -description: read multiple bytes from a port
		* -args: uint8_t port address
		* -args: uint8_t* storage for read bytes
		* -args: uint8_t number of bytes to read
		* -returns: none
		*/
		virtual void read (uint8_t*, uint8_t);

		/* read
		* -description: read from a port where the
		* -description: initiation of a read requires args
		* -args: uint8_t port address
		* -args: uint8_t* write args
		* -args: uint8_t number of write args
		* -args: uint8_t* read args
		* -args: uint8_t number of read args
		* -returns: none
		*/
		virtual void read(uint8_t*, uint8_t, uint8_t*, uint8_t);

		///////////////////////////////////////////
		// Simulated Device Methods:
		///////////////////////////////////////////
		// Check if data is still available
		virtual bool DataAvailable();

		// Read data from the buffer (Read from port as device)
		virtual uint8_t ReadData();

		// Write data to the buffer (Write to port as device)
		virtual void WriteResponse( uint8_t );

	private:
		Queue<uint8_t> mReceiveBuffer;  // Data received by Device Handler
		Queue<uint8_t> mSendBuffer;     // Data written by Device Handler

	private:
		SimulatedComPort(const SimulatedComPort& );
		const SimulatedComPort& operator=(const SimulatedComPort&);
};

#include "SimulatedComPort.cpp"

#endif //__SIMULATED_COM_PORT_H__
