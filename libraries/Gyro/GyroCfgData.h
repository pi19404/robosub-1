#ifndef __GYRO_CFG_DATA_H__
#define __GYRO_CFG_DATA_H__

#include <stdint.h>
#include "..\Device\DeviceCfgData.h"

// Register Names:
namespace GyroRegNames {
enum Enum {
     WhoAmI
    ,CtrlReg1
    ,CtrlReg2
    ,CtrlReg3
    ,CtrlReg4
    ,CtrlReg5
    ,Reference
    ,OutTemp
    ,StatusReg
    ,OutXL
    ,OutXH
    ,OutYL
    ,OutYH
    ,OutZL
    ,OutZH
    ,FifoCtrlReg
    ,FifoSrcReg
    ,Int1Cfg
    ,Int1Src
    ,Int1ThreshXL
    ,Int1ThreshXH
    ,Int1ThreshYL
    ,Int1ThreshYH
    ,Int1ThreshZL
    ,Int1ThreshZH
    ,Int1Duration
    ,NumRegisters
};
}

// GyroCfgData
struct GyroCfgData {
    uint8_t Registers[GyroRegNames::NumRegisters];
}
#define GYRO_REG(reg,data) ((data).Registers[GyroRegNames::reg])

// Register Mode Definitions
const static RegStatus GyroRegStats[] = { 
     { 0x0F, RegMode::ReadOnly  }
    ,{ 0x20, RegMode::ReadWrite }
    ,{ 0x21, RegMode::ReadWrite }
    ,{ 0x22, RegMode::ReadWrite }
    ,{ 0x23, RegMode::ReadWrite }
    ,{ 0x24, RegMode::ReadWrite }
    ,{ 0x25, RegMode::ReadWrite }
    ,{ 0x26, RegMode::ReadOnly  }
    ,{ 0x27, RegMode::ReadOnly  }
    ,{ 0x28, RegMode::ReadOnly  }
    ,{ 0x29, RegMode::ReadOnly  }
    ,{ 0x2A, RegMode::ReadOnly  }
    ,{ 0x2B, RegMode::ReadOnly  }
    ,{ 0x2C, RegMode::ReadOnly  }
    ,{ 0x2D, RegMode::ReadOnly  }
    ,{ 0x2E, RegMode::ReadWrite }
    ,{ 0x2F, RegMode::ReadOnly  }
    ,{ 0x30, RegMode::ReadWrite }
    ,{ 0x31, RegMode::ReadOnly  }
    ,{ 0x32, RegMode::ReadWrite }
    ,{ 0x33, RegMode::ReadWrite }
    ,{ 0x34, RegMode::ReadWrite }
    ,{ 0x35, RegMode::ReadWrite }
    ,{ 0x36, RegMode::ReadWrite }
    ,{ 0x37, RegMode::ReadWrite }
    ,{ 0x38, RegMode::ReadWrite }
};

// CtrlReg1

    // Axis
    struct GyroAxis 
    {
        static const uint8_t X   = BITMSK(0);
        static const uint8_t Y   = BITMSK(1);
        static const uint8_t Z   = BITMSK(2);
        static const uint8_t All = 0b111;
        
        static inline uint8_t ValidateMask( uint8_t mask )
        {
            return (mask & All);
        }
    };
    const uint8_t GyroAxis::X;
    const uint8_t GyroAxis::Y;
    const uint8_t GyroAxis::Z;
    const uint8_t GyroAxis::All;

    // Power Mode
    struct GyroPowerMode
    {
        static const uint8_t Bit = BITMSK(3);
        
        static inline uint8_t ValidateMask( uint8_t mask )
        {
            return (mask & Bit);
        }
    };
    const uint8_t GyroPowerMode::Bit;

    // Data Rate
    struct GyroDataRate 
    {
        static const uint8_t _100Hz = 0;
        static const uint8_t _200Hz = (0b01 << 6);
        static const uint8_t _400Hz = (0b10 << 6);
        static const uint8_t _800Hz = (0b11 << 6);
        
        static inline uint8_t ValidateMask 
    };
    }
    #define GYRO_DATA_RATE_BITS (0b11 << 6)
    
    // Bandwith
    namespace GyroOdr_100Hz_Cutoff {
    enum Enum {
         _12_5Hz
        ,_25Hz
        ,_25Hz
        ,_25Hz
    };
    }
    namespace GyroOdr_200Hz_Cutoff {
    enum Enum {
         _12_5Hz
        ,_25Hz
        ,_50Hz
        ,_70Hz
    };
    }
    namespace GyroOdr_400Hz_Cutoff {
    enum Enum {
         _20Hz
        ,_25Hz
        ,_50Hz
        ,_110Hz
    };
    }
    namespace GyroOdr_800Hz_Cutoff {
    enum Enum {
         _30Hz
        ,_35Hz
        ,_50Hz
        ,_110Hz
    };
    }
    #define GYRO_BANDWIDTH_BITS (0b11 << 4)

    // MACROS
    #define GYRO_ENABLE_AXIS(a,data) ( (GYRO_REG(CtrlReg1,data) &= ~(GYRO_AXIS_BITS)) |= (a) )
    #define GYRO_POWER_MODE(pm,data) ( (GYRO_REG(CtrlReg1,data) &= ~(GYRO_POWER_MODE)) |= (pm) )
    #define GYRO_DATA_RATE(dr,data)  ( (GYRO_REG(CtrlReg1,data) &= ~(GYRO_DATA_RATE_BITS)) |= (dr) )
    #define GYRO_BANDWIDTH(bw,data)  ( (GYRO_REG(CtrlReg1,data) &= ~(GYRO_BANDWIDTH_BITS)) |= (bw) )

// CtrlReg2

    // High-Pass Filter Mode
    namespace GyroHpfMode {
    enum Enum {
         NormalWithReset        = 0
        ,ReferenceSignal        = (0b01 << 4)
        ,NormalMode             = (0b10 << 4)
        ,AutoresetOnInterrupt   = (0b11 << 4)
    };
    }
    #define GYRO_HPF_MODE_BITS (0b11 << 4)
    
    // High-Pass Filter Cutoff Frequency
    namespace GyroOdr_100Hz_Hpf_Cutoff {
    enum Enum {
         _8Hz
        ,_4Hz
        ,_2Hz
        ,_1Hz
        ,_0_5Hz // 0.5Hz
        ,_0_2Hz
        ,_0_1Hz
        ,_0_05Hz // 0.05Hz
        ,_0_02Hz
        ,_0_01Hz
    };
    }
    namespace GyroOdr_200Hz_Hpf_Cutoff {
    enum Enum {
         _15Hz
        ,_8Hz
        ,_4Hz
        ,_2Hz
        ,_1Hz
        ,_0_5Hz // 0.5Hz
        ,_0_2Hz
        ,_0_1Hz
        ,_0_05Hz // 0.05Hz
        ,_0_02Hz
    };
    }
    namespace GyroOdr_400Hz_Hpf_Cutoff {
    enum Enum {
         _30Hz
        ,_15Hz
        ,_8Hz
        ,_4Hz
        ,_2Hz
        ,_1Hz
        ,_0_5Hz // 0.5Hz
        ,_0_2Hz
        ,_0_1Hz
        ,_0_05Hz // 0.05Hz
    };
    }
    namespace GyroOdr_400Hz_Hpf_Cutoff {
    enum Enum {
         _56Hz
        ,_30Hz
        ,_15Hz
        ,_8Hz
        ,_4Hz
        ,_2Hz
        ,_1Hz
        ,_0_5Hz // 0.5Hz
        ,_0_2Hz
        ,_0_1Hz
    };
    }
    #define GYRO_HPF_CUTOFF_BITS (0b1111)  
    
    // MACROS
    #define GYRO_HPF_MODE(mode,data)   ( (GYRO_REG(CtrlReg2,data) &= ~(GYRO_HPF_MODE_BITS)) |= (mode) )
    #define GYRO_HPF_CUTOFF(freq,data) ( (GYRO_REG(CtrlReg2,data) &= ~(GYRO_HPF_CUTOFF_BITS)) |= (freq) )

    
//CtrlReg3

    // Bits
    namespace GyroIntMask {
    enum Enum {
         Int2_FifoEmptyEn  = BITMSK(0)
        ,Int2_OverrunEn    = BITMSK(1)
        ,Int2_WatermarkEn  = BITMSK(2)
        ,Int2_DataReadyEn  = BITMSK(3)
        ,PushPullOpenDrain = BITMSK(4)
        ,Int1_ActiveLow    = BITMSK(5)
        ,Int1_BootEn       = BITMSK(6)
        ,Int1En            = BITMSK(7)
        ,All               = 0xFF
    };
    }
    
    // MACROS
    #define GYRO_SET_INTERRUPT(msk,data) ( GYRO_REG(CtrlReg3,data) |= (msk) )
    #define GYRO_CLR_INTERRUPT(msk,data) ( GYRO_REG(CtrlReg3,data) &= ~(msk) )

//CtrlReg4

    // Block Data Update
    namespace GyroBlockDataUpdate {
    enum Enum {
         Continuous = 0
        ,FullRead   = BITMSK(7) // update when have both MSB and LSB
    };
    }
    #define GYRO_BDU BITMSK(7)
    
    // Endianness
    namespace GyroDataEndianness {
    enum Enum {
         LittleEndian = 0
        ,BigEndian    = BITMSK(6)
    };
    }
    #define GYRO_ENDIANNESS BITMSK(6)
    
    // Full Scale Selection
    namespace GyroFullScaleSelection {
    enum Enum {
         _250dps    = (0b00 << 4)// 250 degrees per second
        ,_500dps    = (0b01 << 4)
        ,_2000dps   = (0b10 << 4)
    };
    }
    #define GYRO_FULL_SCALE_PINS (0b11 << 4)
    
    // Self Test
    namespace GyroSelfTestEnable {
    enum Enum {
         NormalMode       = (0b00 << 1)
        ,SelfTestPositive = (0b01 << 1)
        ,SelfTestNegative = (0b11 << 1)
    };
    }
    #define GYRO_SELF_TEST_PINS (0b11 << 1)
    
    namespace GyroSpiInterfaceMode {
    enum Enum {
         _4Wire
        ,_3Wire
    };
    }
    #define GYRO_SPI_INTERFACE_MODE BITMSK(0)

    // MACROS
    #define GYRO_BLOCK_DATA_UPDATE(bdu,data)    ( (GYRO_REG(CtrlReg4,data) &= ~(GYRO_BDU)) |= (bdu) )
    #define GYRO_DATA_SELECTION(sel,data)       ( (GYRO_REG(CtrlReg4,data) &= ~(GYRO_ENDIANNESS)) |= (sel) )
    #define GYRO_FULL_SCALE_SELECTION(sel,data) ( (GYRO_REG(CtrlReg4,data) &= ~(GYRO_FULL_SCALE_PINS)) |= (sel) )
    #define GYRO_SELF_TEST(mode,data)           ( (GYRO_REG(CtrlReg4,data) &= ~(GYRO_SELF_TEST_PINS)) |= (mode) )
    #define GYRO_SPI_INTERFACE(mode,data)       ( (GYRO_REG(CtrlReg4,data) &= ~(GYRO_SPI_INTERFACE_MODE)) |= (mode) )

//CtrlReg5

    // Bits
    #define GYRO_BOOT_MODE     BITMSK(7)
    #define GYRO_FIFO_ENABLE   BITMSK(6)
    #define GYRO_HPF_ENABLE    BITMSK(4)
    #define GYRO_INT1_SEL0     BITMSK(2)
    #define GYRO_INT1_SEL1     BITMSK(3)
    #define GYRO_INT1_SEL_BITS (0b11 << 2)
    #define GYRO_OUT_SEL0      BITMSK(0)
    #define GYRO_OUT_SEL1      BITMSK(1)
    #define GYRO_OUT_SEL_BITS  (0b11 << 0)
    
    // MACROS
    #define GYRO_REBOOT_MEMORY_CONTENT(yesno,data) ( (GYRO_REG(CtrlReg5,data) &= ~(GYRO_BOOT_MODE)) |= ((yesno) ? GYRO_BOOT_MODE : 0) )
    #define GYRO_FIFO_ENABLE(yesno,data)           ( (GYRO_REG(CtrlReg5,data) &= ~(GYRO_FIFO_ENABLE)) |= ((yesno) ? GYRO_FIFO_ENABLE : 0) )
    #define GYRO_DATA_FIFO_NO_HPF(data) ( GYRO_REG(CtrlReg5,data) &= ~(GYRO_OUT_SEL_BITS) )
    #define GYRO_DATA_FIFO_HPF(data)    ( GYRO_DATA_FIFO_NO_HPF(data) |= GYRO_OUT_SEL0 )
    #define GYRO_DATA_FIFO_LPF(data)    ( (GYRO_REG(CtrlReg5,data) &= ~(GYRO_HPF_ENABLE)) |= GYRO_OUT_SEL1 )
    #define GYRO_DATA_FIFO_LPF_HPF(data)( GYRO_REG(CtrlReg5,data) |= (GYRO_HPF_ENABLE | GYRO_OUT_SEL0) )
    #define GYRO_INT_GEN_NO_HPF(data)   ( GYRO_REG(CtrlReg5,data) &= ~(GYRO_INT1_SEL_BITS) )
    #define GYRO_INT_GEN_HPF(data)      ( GYRO_INT_GEN_NO_HPF(data) |= GYRO_INT1_SEL0 )
    #define GYRO_INT_GEN_LPF(data)      ( (GYRO_REG(CtrlReg5,data) &= ~(GYRO_HPF_ENABLE) ) |= GYRO_INT_SEL1 )
    #define GYRO_INT_GEN_LPF_HPF(data)  ( GYRO_REG(CtrlReg5,data) |= (GYRO_HPF_ENABLE | GYRO_INT_SEL1) )
    
//StatusReg
    
    // Bit Masks
    namespace GyroStatus {
    enum Enum {
        XDA    = BITMSK(0)
       ,YDA    = BITMSK(1)
       ,ZDA    = BITMSK(2)
       ,ZYXDA  = BITMSK(3)
       ,XOR    = BITMSK(4)
       ,YOR    = BITMSK(5)
       ,ZOR    = BITMSK(6)
       ,ZYXOR  = BITMSK(7)
    };
    }
 
//FifoCtrlReg

    // Fifo Mode
    namespace FifoMode {
    enum {
         Bypass         =  0b000
        ,Fifo           = (0b001 << 5)
        ,Stream         = (0b010 << 5)
        ,StreamToFifo   = (0b011 << 5)
        ,BypassToStream = (0b100 << 5)
    };
    }
    #define GYRO_FIFO_MODE_BITS (0b111 << 5)
    
    // Watermark Thresh
    #define GYRO_WATERMARK_BITS 0b11111
    
    //MACROS
    #define GYRO_FIFO_MODE(mode,data) ( (GYRO_REG(FifoCtrlReg,data) &= ~(GYRO_FIFO_MODE_BITS)) |= ((mode)<<5) )
    #define GYRO_WATERMARK(wmk,data)  ( (GYRO_REG(FifoCtrlReg,data) &= ~(GYRO_WATERMARK_BITS)) |= (wmk) )

//FifoSrcReg

    // MACROS
    #define GYRO_FIFO_SRC_WTMK(data)  ( GYRO_REG(FifoCtrlReg,data) & BITMSK(7) )
    #define GYRO_FIFO_SRC_OVRN(data)  ( GYRO_REG(FifoCtrlReg,data) & BITMSK(6) )
    #define GYRO_FIFO_SRC_EMPTY(data) ( GYRO_REG(FifoCtrlReg,data) & BITMSK(5) )
    #define GYRO_FIFO_DATA_LEVEL(data)( GYRO_REG(FifoCtrlReg,data) & ~(0b111 << 6) )    

//Int1Cfg

    // Bit Masks
    namespace GyroInt1Cfg {
    enum Enum {
         XLIE   = BITMSK(0) // XL below thresh
        ,XHIE   = BITMSK(1) // XH above thresh
        ,YLIE   = BITMSK(2)
        ,YHIE   = BITMSK(3)
        ,ZLIE   = BITMSK(4)
        ,ZHIE   = BITMSK(5)
        ,LIR    = BITMSK(6) // Latch int req
        ,AndOr  = BITMSK(7) // And = 1, Or = 1
    };
    }

//Int1Src

    // Bit Masks
    namespace GyroInt1Src {
    enum Enum {
         XL   = BITMSK(0) // XL below thresh
        ,XH   = BITMSK(1) // XH above thresh
        ,YL   = BITMSK(2)
        ,YH   = BITMSK(3)
        ,ZL   = BITMSK(4)
        ,ZH   = BITMSK(5)
        ,IA   = BITMSK(6) // Interrupt Active
    };
    }

//Int1Duration

    // Wait
    #define GYRO_INT1_WAIT BITMSK(7)
    
    // Duration
    #define GYRO_INT1_DURATION_BITS 0x7F

    // MACROS
    #define GYRO_INT1_WAIT(yesno,data) ( ( GYRO_REG(Int1Duration,data) &= ~(GYRO_INT1_WAIT) ) |= ((yesno)<<7) )
    #define GYRO_INT1_DURATION(dur,data) ( (GYRO_REG(Int1Duration,data) &= ~(GYRO_INT1_DURATION_BITS) ) |= (dur) )

#endif //__GYRO_CFG_DATA_H__