#ifndef __SPI_COM_PORT_H__
#define __SPI_COM_PORT_H__

#include <ComPort/ComPort.h>
#include <SPI.h>

#if defined (ARDUINO) && (ARDUINO >= 100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


class SpiComPort
	:
	public ComPort
{
	public:
		SpiComPort( SPIClass& spi, uint8_t slaveSelectPin );
		~SpiComPort();

		/* write
		* -description: write multiple bytes to a port
		* -args: uint8_t port address
		* -args: uint8_t* bytes to write
		* -args: uint8_t number of bytes to write
		* -returns: none
		*/
		virtual void    write(uint8_t*, uint8_t);

		/* read
		* -description: read multiple bytes from a port
		* -args: uint8_t port address
		* -args: uint8_t* storage for read bytes
		* -args: uint8_t number of bytes to read
		* -returns: none
		*/
		virtual void    read (uint8_t*, uint8_t);

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

	private:
		SpiComPort(const SpiComPort&);
		const SpiComPort& operator=(const SpiComPort&);

	private:
		SPIClass& mSpi;
};

#include "SpiComPort.cpp"

#endif //__SPI_COM_PORT_H__
