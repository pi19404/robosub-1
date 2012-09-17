#ifndef __COM_PORT_H__
#define __COM_PORT_H__

#include <Utility/NoCopy.h>
#include <stddef.h>
#include <stdint.h>

#if (ARDUINO >= 100)
  #define __COM_WRITE_RET size_t
#else
  #define __COM_WRITE_RET void
#endif

// Device ComPort Interface Class
class ComPort
    :
    NoCopy
{
 public:

       ComPort( uint8_t device )
        :
        mDevice(device)
       { }
      
      /* write
       * -description: write multiple bytes to a port
           * -args: uint8_t* bytes to write
       * -args: uint8_t number of bytes to write
       * -returns: none
       */
      virtual void    write(uint8_t*, uint8_t) = 0;
      
      /* read
       * -description: read multiple bytes from a port
           * -args: uint8_t* storage for read bytes
       * -args: uint8_t number of bytes to read
       * -returns: none
       */
      virtual void    read (uint8_t*, uint8_t) = 0;
      
      /* read
       * -description: read from a port where the
       * -description: initiation of a read requires args
           * -args: uint8_t* write args
       * -args: uint8_t number of write args
       * -args: uint8_t* read args
       * -args: uint8_t number of read args
       * -returns: none
       */
      virtual void read(uint8_t*, uint8_t, uint8_t*, uint8_t) = 0;
      
      /* getDevice
       * -description: return device address
       * -returns: device address
       */
      uint8_t getDevice( void ){ return mDevice; }
  
  protected:
    uint8_t mDevice;

  private:
        ComPort(const ComPort&);
        const ComPort& operator=(const ComPort&);
};

#endif //__COM_PORT_H__
