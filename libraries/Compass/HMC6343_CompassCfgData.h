#ifndef __HMC6343_COMPASS_CFG_DATA_H__
#define __HMC6343_COMPASS_CFG_DATA_H__

#include "..\Device\DeviceCfgData.h"
#include <stdint.h>

namespace CompassRegNames {
enum Enum {
     GetAccelData
    ,GetMagnetoData
    ,GetHeadingData
    ,GetTiltData
    ,GetConfig
    ,EnterCalibrationMode
    ,SetOrientationLevel
    ,SetOrientationUprightEdge
    ,SetOrientationUprightFront
    ,EnterRunMode
    ,EnterStandbyMode
    ,ExitCalibrationMode
    ,ResetDevice
    ,EnterSleepMode
    ,ExitSleepMode
    ,EepromRead
    ,EepromWrite
};
}

namespace CompassEepromRegNames {
enum Enum {
     Config
    ,Rate
    ,HeadingFilter
};
}

// EEPROM Registers

    //Config Register
    namespace CompassOrientation {
    enum Enum {
         Level        = BITMSK(0)
        ,UprightEdge  = BITMSK(1)
        ,UprightFront = BITMSK(2)
    };
    }

    class CompassConfig
        :
        public Register
    {
        public: 
            CompassConfig(uint8_t reg = 0) : Register(reg) { }
            
            using Register::GetValue;
            
            void SetOrientation( CompassOrientation::Enum orientation )
            {
                using namespace CompassOrientation;
                uint8_t oMask = (Level | UprightEdge | UprightFront);
                
                ( (mValue &= ~(oMask)) |= orientation );
            }
            
            bool IsInStandby()
            {
                return (mValue & BITMSK(3));
            }
            
            bool IsRunning()
            {
                return (mValue & BITMSK(4));
            }
            
            bool IsHeadingFilterEnabled()
            {
                return (mValue & BITMSK(5));
            }
            
            void EnableHeadingFilter( bool yesno )
            {
                ( (mValue &= ~BITMSK(5)) |= (yesno << 5));
            }

            bool IsCalibrating()
            {
                return (mValue & BITMSK(6));
            }
            
            bool IsCalculatingData()
            {
                return (mValue & BITMSK(7));
            }
    };

    // Rate Register  
    class CompassRate
        :
        public Register
    {
        public:
            enum Enum {
                _1Hz  = 0b00
               ,_5Hz  = 0b01
               ,_10Hz = 0b10
            };
            
            CompassRate( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            Enum GetDeviceRate()
            {
                uint8_t rMask = (_5Hz | _10Hz);
                
                return (Enum)(mValue & rMask);
            }
            void SetDeviceRate( Enum rate )
            {
                uint8_t rMask = (_5Hz | _10Hz);
                
                ( (mValue &= ~(rMask)) |= rate );
            }
    };

const static RegStatus CompassEepromStats [] = {
         { 0x04, RegMode::ReadWrite }
        ,{ 0x05, RegMode::ReadWrite }
        ,{ 0x14, RegMode::ReadWrite }
};

const static RegStatus CompassRegStats [] = {
         { 0x40, RegMode::ReadOnly }
        ,{ 0x45, RegMode::ReadOnly }
        ,{ 0x50, RegMode::ReadOnly }
        ,{ 0x55, RegMode::ReadOnly }
        ,{ 0x65, RegMode::ReadOnly }
        ,{ 0x71, RegMode::Command  }
        ,{ 0x72, RegMode::Command  }
        ,{ 0x73, RegMode::Command  }
        ,{ 0x74, RegMode::Command  }
        ,{ 0x75, RegMode::Command  }
        ,{ 0x76, RegMode::Command  }
        ,{ 0x7E, RegMode::Command  }
        ,{ 0x82, RegMode::Command  }
        ,{ 0x83, RegMode::Command  }
        ,{ 0x84, RegMode::Command  }
        ,{ 0xE1, RegMode::Command  }
        ,{ 0xF1, RegMode::Command  }
};

#endif //__HMC6343_COMPASS_CFG_DATA_H__