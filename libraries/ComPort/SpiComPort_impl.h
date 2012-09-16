#ifndef __SPI_COM_PORT_IMPL_H__
#define __SPI_COM_PORT_IMPL_H__


#ifndef __SPI_COM_PORT_H__
    #error This file must be included from SpiComPort.h
#endif

SpiComPort::SpiComPort(SPIClass& spi, uint8_t slaveSelectPin)
	:
	 ComPort(slaveSelectPin)
	,mSpi(spi)
{
  // Set slave select pin as output
  pinMode(mDevice, OUTPUT);
  digitalWrite(mDevice, HIGH);
}

SpiComPort::~SpiComPort()
{
	// Nothing left to do here.
}

void SpiComPort::write(uint8_t* data, uint8_t nBytes)
{
    digitalWrite(mDevice,LOW);
    for( int i = 0; i < nBytes; ++i )
    {
        mSpi.transfer(data[i]);
    }
    digitalWrite(mDevice, HIGH);
}

void SpiComPort::read(uint8_t* data, uint8_t nBytes)
{
  read(0, 0, data, nBytes );
}

void SpiComPort::read(uint8_t* wrdata, uint8_t w_nBytes, uint8_t* rddata, uint8_t r_nBytes)
{
  // Initiate Communication
  digitalWrite(mDevice, LOW);
  if( r_nBytes     ){ wrdata[0] |= 0x80; }
  if( r_nBytes > 1 ){ wrdata[0] |= 0x40; }
  
  // Do write part
  for( int i = 0; i < w_nBytes; ++i )
  {
    mSpi.transfer( wrdata[i] );
  }
  
  // Do read part
  for( int i = 0; i < r_nBytes; ++i )
  {
    rddata[i] = mSpi.transfer(0x00);
  }
  
  // End Communication with Device
  digitalWrite(mDevice, HIGH);
}

#endif //__SPI_COM_PORT_IMPL_H__
