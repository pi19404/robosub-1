#ifndef __SERIAL_COMPORT_IMPL_H__
#define __SERIAL_COMPORT_IMPL_H__

#ifndef __SERIAL_COM_PORT_H__
    #error This file must be included by SerialComPort.h
#endif

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include "../Logging/LogManager.h"

SerialComPort::SerialComPort( HardwareSerial& serial, uint8_t device, unsigned long timeOutMillis )
  :
   ComPort(device)
  ,mSerial(serial)
  ,mTimeOut(timeOutMillis)
{
	// Nothing left to do here.
}

SerialComPort::~SerialComPort()
{
	// Nothing left to do here.
}

void SerialComPort::write(uint8_t *data, uint8_t nBytes)
{
  for( int i = 0; i < nBytes; ++i )
  {
    mSerial.write(data[i]);
  }
}

void SerialComPort::read(uint8_t *data, uint8_t nBytes)
{    
    read(0, 0, data, nBytes);
}

void SerialComPort::read(uint8_t* wrdata, uint8_t w_nBytes, uint8_t* rddata, uint8_t r_nBytes)
{
    // Do write part
    for( int i = 0; i < w_nBytes; ++i )
    {
        mSerial.write( wrdata[i] );
    }
    
    // Do read part
    int i;
    for( i = 0; i < r_nBytes; ++i )
    {   
        while( !mSerial.available() );
        rddata[i] = mSerial.read();
    }
}

#endif //__SERIAL_COMPORT_IMPL_H__
