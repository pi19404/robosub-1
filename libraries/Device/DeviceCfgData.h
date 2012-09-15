#ifndef __DEVICE_CFG_DATA_H__
#define __DEVICE_CFG_DATA_H__

#include <stdint.h>

#define BITMSK(x) (1 << (x))

// Register status:
namespace RegMode {
enum Enum {
     ReadOnly
    ,ReadWrite
    ,Command
};
}
struct RegStatus {
    int           Addr;
    RegMode::Enum Mode;
};

class Register {

    protected:
        uint8_t mValue;
    
    public:
        Register( uint8_t val  = 0 ) : mValue(val) { }
        uint8_t GetValue( void ) const { return mValue; }
};
        

#endif //__DEVICE_CFG_DATA_H__
