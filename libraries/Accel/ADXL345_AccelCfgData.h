#ifndef __ACCEL_CFG_DATA_H__
#define __ACCEL_CFG_DATA_H__

#include <stdint.h>
#include "../Device/DeviceCfgData.h"

namespace Accel {

// Register names:
namespace RegNames {
enum Enum
{
	 DeviceId
	,TapThreshold
	,OffsetX
	,OffsetY
	,OffsetZ
	,TapDuration
	,TapLatency
	,TapWindow
	,ActivityThreshold
	,InactivityThreshold
	,InactivityTime
	,ActInactControl
	,FreeFallThreshold
	,FreeFallTime
	,TapAxesControl
	,ActTapSrc
	,DataRatePwrModeCtrl
	,PwrCtrl
	,InterruptEnableControl
	,InterruptMapControl
	,InterruptSource
	,DataFormat
	,DataXL
	,DataXH
	,DataYL
	,DataYH
	,DataZL
	,DataZH
	,FifoCtrl
	,FifoStatus
	,NumRegisters
};
}

// Register Mode Definitions
static const RegStatus RegStats[] =	{ 
         { 0x00, RegMode::ReadOnly  }
        ,{ 0x1D, RegMode::ReadWrite }
        ,{ 0x1E, RegMode::ReadWrite }
        ,{ 0x1F, RegMode::ReadWrite }
        ,{ 0x20, RegMode::ReadWrite }
        ,{ 0x21, RegMode::ReadWrite }
        ,{ 0x22, RegMode::ReadWrite }
        ,{ 0x23, RegMode::ReadWrite }
        ,{ 0x24, RegMode::ReadWrite }
        ,{ 0x25, RegMode::ReadWrite }
        ,{ 0x26, RegMode::ReadWrite }
        ,{ 0x27, RegMode::ReadWrite }
        ,{ 0x28, RegMode::ReadWrite }
        ,{ 0x29, RegMode::ReadWrite }
        ,{ 0x2A, RegMode::ReadWrite }
        ,{ 0x2B, RegMode::ReadOnly  }
        ,{ 0x2C, RegMode::ReadWrite }
        ,{ 0x2D, RegMode::ReadWrite }
        ,{ 0x2E, RegMode::ReadWrite }
        ,{ 0x2F, RegMode::ReadWrite }
        ,{ 0x30, RegMode::ReadOnly  }
        ,{ 0x31, RegMode::ReadWrite }
        ,{ 0x32, RegMode::ReadOnly  }
        ,{ 0x33, RegMode::ReadOnly  }
        ,{ 0x34, RegMode::ReadOnly  }
        ,{ 0x35, RegMode::ReadOnly  }
        ,{ 0x36, RegMode::ReadOnly  }
        ,{ 0x37, RegMode::ReadOnly  }
        ,{ 0x38, RegMode::ReadWrite }
        ,{ 0x39, RegMode::ReadOnly  }
};
    
// Conversion Macros
#define ACCEL_SCALE_VAL(x)             ( (int16_t(x) < 0x80 ) ? 0x80 : ( (int16_t(x) > 0x7F) ? 0x7F : int16_t(x) ) )
#define ACCEL_OFFSET_mG_TO_VAL(x)      ( (uint8_t)ACCEL_SCALE_VAL( (x) / 15.6   ) )
#define ACCEL_TAP_DUR_ms_TO_VAL(x)     ( (uint8_t)ACCEL_SCALE_VAL( (x) /  0.625 ) )    
#define ACCEL_TAP_LAT_ms_TO_VAL(x)     ( (uint8_t)ACCEL_SCALE_VAL( (x) /  1.25  ) )
#define ACCEL_TAP_WINDOW_ms_TO_VAL(x)  ( (uint8_t)ACCEL_SCALE_VAL( (x) /  1.25  ) )
#define ACCEL_TAP_THRESH_mG_TO_VAL(x)  ( (uint8_t)ACCEL_SCALE_VAL( (x) / 62.5   ) )
#define ACCEL_ACT_THRESH_mG_TO_VAL(x)  ( (uint8_t)ACCEL_SCALE_VAL( (x) / 62.5   ) )
#define ACCEL_INACT_THRESH_mG_TO_VAL(x)( (uint8_t)ACCEL_SCALE_VAL( (x) / 62.5   ) )
#define ACCEL_TIME_INACT_s_TO_VAL(x)   ( (uint8_t)ACCEL_SCALE_VAL( (x) )          )
#define ACCEL_FF_THRESH_mG_TO_VAL(x)   ( (uint8_t)ACCEL_SCALE_VAL( (x) /  5.0   ) )
#define ACCEL_FF_TIME_ms_TO_VAL(x)     ( (uint8_t)ACCEL_SCALE_VAL( (x) / 62.5   ) )


// ActInactControlReg
    
    // Bit Masks
    namespace ActivityCoupling {
    enum Enum {
         DcCoupled = 0
        ,AcCoupled = BITMSK(7)
    };
    }
    
    namespace InactivityCoupling {
    enum Enum {
         DcCoupled = 0
        ,AcCoupled = BITMSK(3)
    };
    }
    
    namespace ActivityAxis {
    enum Enum {
         X = BITMSK(6)
        ,Y = BITMSK(5)
        ,Z = BITMSK(4)
    };
    }
    
    namespace InactivityAxis {
    enum Enum {
         X = BITMSK(2)
        ,Y = BITMSK(1)
        ,Z = BITMSK(0)
    };
    }
    
    class ActInactCtrlReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            ActInactCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            void SetActivityCoupling( ActivityCoupling::Enum en )
            {
                uint8_t mask = ActivityCoupling::AcCoupled;
                ( mValue &= ~mask ) |= en;
            }
            
            void SetInactivityCoupling( InactivityCoupling::Enum en )
            {
                uint8_t mask = InactivityCoupling::AcCoupled;
                ( mValue &= ~mask ) |= en;
            }
            
            void EnableActivityDetection( ActivityAxis::Enum en )
            {
                mValue |= en;
            }
            
            void DisableActivityDetection( ActivityAxis::Enum en )
            {
                mValue &= ~en;
            }
            
            void EnableInactivityDetection( InactivityAxis::Enum en )
            {
                mValue |= en;
            }
            
            void DisableInactivityDetection( InactivityAxis::Enum en )
            {
                mValue &= ~en;
            }
            
            void SetActivityDetection( uint8_t mask )
            {
                using namespace ActivityAxis;
                uint8_t fmask = (X | Y | Z);
                ( mValue &= ~fmask ) |= (fmask & mask);
            }
            
            void SetInActivityDetection( uint8_t mask )
            {
                using namespace InactivityAxis;
                uint8_t fmask = (X | Y | Z);
                ( mValue &= ~fmask ) |= (fmask & mask);
            }
    };
    
// TapAxesCtrl
    
    // Bit Masks
    namespace TapAxes {
    enum Enum {
         None = 0
        ,Z = BITMSK(0)
        ,Y = BITMSK(1)
        ,X = BITMSK(2)
    };
    }
    
    namespace DoubleTapDetection {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(3)
    };
    }
    
    class TapAxesCtrlReg 
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            TapAxesCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            void EnableTapAxis( TapAxes::Enum axis )
            {
                mValue |= axis;
            }
            
            void DisableTapAxis( TapAxes::Enum axis )
            {
                mValue &= ~axis;
            }
            
            void SetTapAxes( uint8_t mask )
            {
                using namespace TapAxes;
                uint8_t fmask = (X | Y | Z);
                ( mValue &= ~fmask ) |= (fmask & mask);
            }
            
            void SetDoubleTapDetection( DoubleTapDetection::Enum en )
            {
                using namespace DoubleTapDetection;
                ( mValue &= ~Yes ) |= en;
            }
    };
    
// ActTapSrc
    
    // Bit Masks
    namespace ActSrcAxis {
    enum Enum {
         X = BITMSK(6)
        ,Y = BITMSK(5)
        ,Z = BITMSK(4)
    };
    }
    
    namespace TapSrcAxis {
    enum Enum {
         X = BITMSK(2)
        ,Y = BITMSK(1)
        ,Z = BITMSK(0)
    };
    }
    
    namespace DeviceAsleep {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(3)
    };
    }
    
    class ActTapSrcReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            ActTapSrcReg( uint8_t reg = 0 ) : Register( reg ) { }
            
            bool IsDeviceAsleep()
            {
                return (mValue & DeviceAsleep::Yes);
            }
            
            bool IsTapSrc( TapSrcAxis::Enum en )
            {
                return (mValue & en);
            }
            
            bool IsTapSrc( uint8_t mask )
            {
                using namespace TapSrcAxis;
                uint8_t fmask = (X | Y | Z);
                return (mValue & (fmask & mask) );
            }
            
            bool IsActSrc( ActSrcAxis::Enum en )
            {
                return (mValue & en);
            }
            
            bool IsActSrc( uint8_t mask )
            {
                using namespace ActSrcAxis;
                uint8_t fmask = (X | Y | Z);
                return (mValue & (fmask & mask));
            }
    };
    
// DataRateDataRatePwrModeCtrl
    
    // Bit Masks
    namespace LowPower {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(4)
    };
    }
    
    namespace DataRate {
    enum Enum {
         _100mHz    // 100 mHz or 0.1 Hz
        ,_200mHz
        ,_390mHz
        ,_780mHz
        
        ,_1_56Hz    // 1.56 Hz
        ,_3_13Hz
        ,_6_25Hz
        
        ,_12_5Hz
        ,_25Hz
        ,_50Hz
        
        ,_100Hz
        ,_200Hz
        ,_400Hz
        ,_800Hz
        
        ,_1_6KHz    // 1.6 KHz or 1600 Hz
        ,_3_2KHz
    };
    }
    
    class DataRatePwrModeCtrlReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            DataRatePwrModeCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            void EnableLowPowerMode( LowPower::Enum en )
            {
                using namespace LowPower;
                ( mValue &= ~Yes ) |= en;
            }
            
            void SetDataRate( DataRate::Enum en )
            {
                uint8_t mask = 0x0F;
                ( mValue &= ~mask ) |= en;
            }
    };
    
    
// PwrCtrl

    // Bit Masks
    namespace ActInactMode {
    enum Enum {
         Separate = 0
        ,Linked   = BITMSK(5)
    };
    }
    
    namespace AutoSleep {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(4)
    };
    }
    
    namespace MeasurementMode {
    enum Enum {
         Standby = 0
        ,Run     = BITMSK(3)
    };
    }
    
    namespace SleepMode {
    enum Enum {
         StandbyRun = 0
        ,Sleep      = BITMSK(2)
    };
    }
    
    namespace WakeupFrequency {
    enum Enum {
         _8Hz
        ,_4Hz
        ,_2Hz
        ,_1Hz
    };
    }
    
    class PwrCtrlReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            PwrCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            void SetActInactMode( ActInactMode::Enum en )
            {
                using namespace ActInactMode;
                (mValue &= ~Linked) |= en;
            }
            
            void EnableAutoSleep( AutoSleep::Enum en )
            {
                using namespace AutoSleep;
                (mValue &= ~Yes) |= en;
            }
            
            void SetMeasurementMode( MeasurementMode::Enum en )
            {
                using namespace MeasurementMode;
                (mValue &= ~Run) |= en;
            }
            
            void SetSleepMode( SleepMode::Enum en )
            {
                using namespace SleepMode;
                (mValue &= ~Sleep) |= en;
            }
            
            void SetWakeupFrequency( WakeupFrequency::Enum en )
            {
                using namespace WakeupFrequency;
                (mValue &= ~_1Hz) |= en;
            }
    };
    

// Interrupts:

    // Bit Masks
    namespace Interrupts {
    enum Enum {
         None       = 0
        ,Overrun    = BITMSK(0)
        ,Watermark  = BITMSK(1)
        ,FreeFall   = BITMSK(2)
        ,Inactivity = BITMSK(3)
        ,Activity   = BITMSK(4)
        ,DoubleTap  = BITMSK(5)
        ,SingleTap  = BITMSK(6)
        ,DataReady  = BITMSK(7)
        ,All        = 0xFF
    };
    }
    
    // Values
    namespace InterruptMap {
    enum Enum {
         Int1 = 0x00
        ,Int2 = 0xFF
    };
    }
    
    // IntEnable Reg
    class IntCtrlReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            IntCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
        
        
            void SetInterrupts( Interrupts::Enum en )
            {
                ( mValue &= ~0xFF ) |= en;
            }
            
            void EnableInterrupt( Interrupts::Enum en )
            {
                mValue |= en;
            }
            
            void DisableInterrupt( Interrupts::Enum en )
            {
                mValue &= ~en;
            }
    };
    
    // IntMap Reg
    class IntMapReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            IntMapReg( uint8_t reg = 0 ) : Register(reg) { }    
        
            void MapInterrupt( Interrupts::Enum en, InterruptMap::Enum map )
            {
                ( mValue &= ~en ) |= (en & map);
            }
            
            void MapInterrupts( uint8_t mask, InterruptMap::Enum map )
            {
                ( mValue &= ~0xFF ) |= (map & mask);
            }
    };
    
    // IntSrc Reg
    class IntSrcReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            IntSrcReg( uint8_t reg = 0 ) : Register(reg) { }
        
            bool IsIntSource( Interrupts::Enum en )
            {
                return (mValue & en);
            }
            
            bool IsIntSource( uint8_t mask )
            {
                return (mValue & mask);
            }
    };
    
// DataFormat Reg

    // Bit Masks
    namespace TestForce {
    enum Enum {
         Off = 0
        ,On  = BITMSK(7)
    };
    }
    
    namespace SpiMode {
    enum Enum {
         _4Wire = 0
        ,_3Wire = BITMSK(6)
    };
    }
    
    namespace InterruptInversion {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(5)
    };
    }
    
    namespace FullResolutionMode {
    enum Enum {
         No  = 0
        ,Yes = BITMSK(3)
    };
    }
    
    namespace Justify {
    enum Enum {
         Right = 0
        ,Left  = BITMSK(2)  // MSB first
    };
    }
    
    namespace MeasurementRange {
    enum Enum {
         _2g    // +/- 2g 
        ,_4g
        ,_8g
        ,_16g
    };
    }
    
    class DataFormatReg 
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            DataFormatReg( uint8_t reg = 0 ) : Register(reg) { }
        
            void SetTestForce( TestForce::Enum en )
            {
                using namespace TestForce;
                ( mValue &= ~On ) |= en;
            }
            
            void SetSpiMode( SpiMode::Enum en )
            {
                using namespace SpiMode;
                ( mValue &= ~_3Wire ) |= en;
            }
            
            void SetInterruptInversion( InterruptInversion::Enum en )
            {
                using namespace InterruptInversion;
                ( mValue &= ~Yes ) |= en;
            }
            
            void SetFullResolutionMode( FullResolutionMode::Enum en )
            {
                using namespace FullResolutionMode;
                ( mValue &= ~Yes ) |= en;
            }
            
            void SetJustify( Justify::Enum en )
            {
                using namespace Justify;
                ( mValue &= ~Left ) |= en;
            }
            
            void SetMeasurementRange( MeasurementRange::Enum en )
            {
                using namespace MeasurementRange;
                ( mValue &= ~_16g ) |= en;
            }             
    };
         
// FifoCtrl Reg

    // Bit Masks
    namespace FifoMode {
    enum Enum {
         Bypass  = 0x00
        ,Fifo    = 0x40
        ,Stream  = 0x80
        ,Trigger = 0xC0
    };
    }
    
    namespace Trigger {
    enum Enum {
         Int1 = 0x00
        ,Int2 = BITMSK(5)
    };
    }
    
    class FifoCtrlReg 
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            FifoCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            void SetFifoMode( FifoMode::Enum en )
            {
                ( mValue &= ~FifoMode::Trigger ) |= en;
            }
            
            void SetTrigger( Trigger::Enum en )
            {
                using namespace Trigger;
                ( mValue &= ~Int2 ) |= en;
            }
            
            void SetWatermarkThreshold( uint8_t threshold )
            {
                ( mValue &= ~0x1F ) |= (threshold & 0x1F);
            }
    };
    
// FifoStatus

    class FifoStatusReg
        :
        public Register
    {
        public:
            using Register::GetValue;
            
            FifoStatusReg( uint8_t reg = 0 ) : Register( reg ) { }
            
            bool IsTriggerOccuring()
            {
                return (mValue & BITMSK(7));
            }
            
            uint8_t GetFifoLevel()
            {
                return (mValue & 0x3F);
            }
    };      
}
#endif //__ACCEL_CFG_DATA_H__
