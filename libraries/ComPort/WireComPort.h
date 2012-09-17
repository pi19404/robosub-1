#ifndef __WIRE_COM_PORT_H__
#define __WIRE_COM_PORT_H__

#include <Wire.h>
#include <ComPort/ComPort.h>

// ComPort Definition for Wire Devices
class WireComPort 
    : 
    public ComPort
{
 private: 
	TwoWire&      mWire;
    unsigned long mTimeOut;
    
 public:
  WireComPort( TwoWire& wire, uint8_t device, unsigned long timeOutMilis = 1500);
  ~WireComPort();
  
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
  
  private:
    WireComPort(const WireComPort&);
    const WireComPort& operator=(const WireComPort&);
};

#endif //__WIRE_COM_PORT_H__
