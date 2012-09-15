#ifndef __ACCEL_CFG_DATA_H__
#define __ACCEL_CFG_DATA_H__

#include <stdint.h>
#include "../Device/DeviceCfgData.h"

// Register names:
namespace AccelRegNames {
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
	,ActTapStatus
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
static const RegStatus AccelRegStats[] =	{ 
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
    
// ActiveInactiveControl register
namespace AccelActInactCtrlBits {
enum Enum {
     InactZ          = BITMSK(0)
    ,InactY          = BITMSK(1)
    ,InactX          = BITMSK(2)
    ,InactAcCouple   = BITMSK(3)
    ,ActZ            = BITMSK(4)
    ,ActY            = BITMSK(5)
    ,ActX            = BITMSK(6)
    ,ActAcCouple     = BITMSK(7)
};
}

// TapAxesControl register
namespace AccelTapAxesCtrlBits {
enum Enum {
     TapZ      = BITMSK(0)
    ,TapY      = BITMSK(1)
    ,TapX      = BITMSK(2)
    ,SupDblTap = BITMSK(3)
};
}

// ActivityTapStatus register (RD ONLY)
namespace AccelActivityTapStatusBits {
enum Enum {
     TapSrcZ = BITMSK(0)
    ,TapSrcY = BITMSK(1)
    ,TapSrcX = BITMSK(2)
    ,Asleep  = BITMSK(3)
    ,ActSrcZ = BITMSK(4)
    ,ActSrcY = BITMSK(5)
    ,ActSrcX = BITMSK(6)
};
}

// DataRatePowerModeControl register
namespace AccelDataRate {
enum Enum
{
	 mHz_00100  = 0b0000
    ,mHz_00200
    ,mHz_00390
    ,mHz_00780
    ,mHz_01560
    ,mHz_03130
    ,mHz_06250
    ,mHz_12500
    ,Hz_0025  
    ,Hz_0050  
    ,Hz_0100  
    ,Hz_0200  
    ,Hz_0400  
    ,Hz_0800  
    ,Hz_1600  
    ,Hz_3200  
};
}   
struct AccelDataRatePwrModeCtrlReg {
    DataRate::Enum Rate         : 4;
    int            LowPowerEn   : 1;
    int            Reserved     : 3;
};

// PowerControl register
namespace AccelWakeupFreq {
enum Enum
{
	 Hz_8 = 0b00
    ,Hz_4
    ,Hz_2
    ,Hz_1
};
}
namespace AccelSleepPwrMode {
enum Enum {
	 Normal
    ,Sleep
};
}
namespace AccelMeasurePwrMode {
enum Enum {
     Standby
    ,Measure
};
}
struct AccelPwrCtrlReg {
    WakeupFreq::Enum     WakeFrequency   : 2;
    SleepPwrMode::Enum   SleepMode       : 1;
    MeasurePwrMode::Enum MeasureMode     : 1;
    int                  AutoSleepEn     : 1;
    int                  LinkEn          : 1;
    int                  Reserved        : 2;
};

// InterruptEnable, InterruptMap, and InterruptSource registers
namespace AccelInterruptBits {
enum Enum {
     Overrun     = BITMSK(0)
    ,Watermark   = BITMSK(1)
    ,FreeFall    = BITMSK(2)
    ,Inactivity  = BITMSK(3)
    ,Activity    = BITMSK(4)
    ,DblTap      = BITMSK(5)
    ,SglTap      = BITMSK(6)
    ,DataReady   = BITMSK(7)
    ,All         = 0xFF
};
}

// DataFormat register
namespace AccelRange {
enum Enum
{
	 g_PlusMinus2   = 0b00
    ,g_PlusMinus4
    ,g_PlusMinus8
    ,g_PlusMinus16
};
}
struct AccelDataFormatReg {
    Range::Enum MeasurementRange    : 2;
    int         JustifyMsbEn        : 1;
    int         FullResEn           : 1;
    int         Reserved            : 1;
    int         InvertIntEn         : 1;
    int         Spi3WireEn          : 1;
    int         SelfTestEn          : 1;
};

// FifoCtrl register
namespace AccelFifoStructMode {
enum Enum {
     Bypass
    ,Fifo
    ,Stream
    ,Trigger
};
}

namespace AccelFifoStructTrigger {
enum Enum {
     Int1
    ,Int2
};
}
struct AccelFifoCtrlReg {
    int                         Samples     : 5;
    FifoStructTrigger::Enum     Trigger     : 1;
    FifoStructMode::Enum        FifoMode    : 2;
};

// FifoStatus register (RD ONLY)
struct AccelFifoStatusReg {
    int NumEntries  : 6;
    int Reserved    : 1;
    int FifoTrig    : 1;
};
                        
union AccelCfgData {
    uint8_t Registers[RegNames::NumRegisters];
    struct {
        uint8_t                     Reserved[16];
        DataRatePwrModeCtrlReg      DataRatePwrModeCtrl;
        PwrCtrlReg                  PwrCtrl;
        uint8_t                     Reserved2[3];
        DataFormatReg               DataFormat;
        uint8_t                     Reserved3[6];
        FifoCtrlReg                 FifoCtrl;
        FifoStatusReg               FifoStatus;
    };
};

#endif //__ACCEL_CFG_DATA_H__
