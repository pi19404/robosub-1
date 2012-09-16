#ifndef __L3G4200D_GYRO_CFG_DATA_H__
#define __L3G4200D_GYRO_CFG_DATA_H__

#include <stdint.h>
#include "../Device/DeviceCfgData.h"

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
    ,Int1ThreshXH
    ,Int1ThreshXL
    ,Int1ThreshYH
    ,Int1ThreshYL
    ,Int1ThreshZH
    ,Int1ThreshZL
    ,Int1Duration
    ,NumRegisters
};
}

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
    
    // Bits
    namespace GyroAxis {
    enum Enum {
         X = 1
        ,Y = 2
        ,Z = 4
        ,Mask = (X | Y | Z)
    };
    }
    namespace GyroPowerMode {
    enum Enum {
         PowerDown = 0
        ,Normal    = 8
        ,Mask      = Normal
    };
    }
    namespace GyroDataRate {
    enum Enum {
         _100Hz = 0
        ,_200Hz = 0x40
        ,_400Hz = 0x80
        ,_800Hz = 0xC0
        ,Mask   = (0x80 | 0x40)
    };
    }
    namespace GyroBandwith {
    enum Enum {
         Zero  = 0
        ,One   = 0x10
        ,Two   = 0x20
        ,Three = 0x30
        ,Mask  = (0x20 | 0x10)
    };
    }
        
    class GyroCtrlReg1
        :
        public Register
    {
        public:
            GyroCtrlReg1( uint8_t reg = 0) : Register(reg) { }
            
            using Register::GetValue;
            
            void EnableAxis( GyroAxis::Enum axis )
            {
                mValue |= axis;
            }
            
            void DisableAxis( GyroAxis::Enum axis )
            {
                mValue &= ~axis;
            }
            
            void SetAxes( uint8_t axes )
            {
                ( mValue &= ~( GyroAxis::Mask ) ) |= ( axes & GyroAxis::Mask );
            }
            
            void ClearAxes( uint8_t axes )
            {
                ( mValue &= ~(GyroAxis::Mask & axes) );
            }

            void EnterPowerDownMode( void )
            {
                ( mValue &= ~( GyroPowerMode::Mask ) ) |= ( GyroPowerMode::PowerDown );
            }
            
            void EnterNormalMode( void )
            {
                ( mValue &= ~( GyroPowerMode::Mask ) ) |= ( GyroPowerMode::Normal );
            }
            
            void SetDataRate( GyroDataRate::Enum rate )
            {
                ( mValue &= ~( GyroDataRate::Mask  ) ) |= rate;
            }
            
            void SetBandwith( GyroBandwith::Enum bw )
            {
                ( mValue &= ~( GyroBandwith::Mask  ) ) |= bw;
            }
    };
   
// CtrlReg2

    // Bits
    namespace GyroHpfDivider {
    enum Enum {
         _12_5
        ,_25
        ,_50
        ,_100
        ,_500
        ,_1K
        ,_2K
        ,_5K
        ,_10K
    };
    }
    namespace GyroHpfMode {
    enum Enum {
         NormalWithReset      = 0x00
        ,ReferenceMode        = 0x10
        ,NormalMode           = 0x20
        ,AutoresetOnInterrupt = 0x30
        ,Mask = (0x20 | 0x10)
    };
    }

    class GyroCtrlReg2
        :
        public Register
    {
        public:
            GyroCtrlReg2( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            void SetHighPassFilterDivider( GyroHpfDivider::Enum div )
            {
                uint8_t Mask = 0x0F;
                
                ( mValue &= ~( Mask ) ) |= div;
            }
            
            void SetHighPassFilterMode( GyroHpfMode::Enum mode )
            {
                ( mValue &= ~( GyroHpfMode::Mask ) ) |= mode;
            }
    };

//CtrlReg3

    // Bits
    namespace GyroInterrupts {
    enum Enum {
         None              = 0x00
        ,Int2_FifoEmptyEn  = BITMSK(0)
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
    
    class GyroCtrlReg3
        :
        public Register
    {
        public:
            GyroCtrlReg3( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            void EnableInterrupt( GyroInterrupts::Enum interrupt )
            {
                mValue |= interrupt;
            }
            
            void DisableInterrupt( GyroInterrupts::Enum interrupt )
            {
                mValue &= ~interrupt;
            }
            
            void SetInterrupts( uint8_t mask )
            {
                mValue = mask;
            }
            
            void ClearInterrupts( uint8_t mask )
            {
                mValue = ~mask;
            }
   };
   
//CtrlReg4

    // Block Data Update
    namespace GyroBlockDataUpdate {
    enum Enum {
         Continuous = 0
        ,FullRead   = BITMSK(7) // update when have both MSB and LSB
        ,Mask       = BITMSK(7)
    };
    }
    // Endianness
    namespace GyroDataSelection {
    enum Enum {
         LittleEndian = 0
        ,BigEndian    = BITMSK(6)
        ,Mask         = BITMSK(6)
    };
    }
    // Full Scale Selection
    namespace GyroFullScaleSelection {
    enum Enum {
         _250dps    = 0// 250 degrees per second
        ,_500dps    = 0x10
        ,_2000dps   = 0x20
    };
    }
    // Self Test
    namespace GyroSelfTest {
    enum Enum {
         Disable          = 0
        ,PositiveValues  = 0x02
        ,NegativeValues  = 0x06
    };
    }
    namespace GyroSpiInterfaceMode {
    enum Enum {
         _4Wire = 0
        ,_3Wire = BITMSK(1)
        ,Mask   = BITMSK(1)
    };
    }
    
    class GyroCtrlReg4 
        :
        public Register
    {
        public:
            GyroCtrlReg4( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            void SetBlockDataUpdateMode( GyroBlockDataUpdate::Enum mode )
            {
                ( mValue &= ~(GyroBlockDataUpdate::Mask) ) |= mode;
            }
            
            void SetDataSelection( GyroDataSelection::Enum sel )
            {
                ( mValue &= ~(GyroDataSelection::Mask) ) |= sel;
            }
            
            void SetFullScaleSelection( GyroFullScaleSelection::Enum ss )
            {
                
                uint8_t Mask = (0x20 | 0x10);
                ( mValue &= ~(Mask) ) |= ss;
            }
            
            void SetSelfTestMode( GyroSelfTest::Enum mode )
            {
                uint8_t Mask = (0x06 | 0x02);
                ( mValue &= ~(Mask) ) |= mode;
            }

            void SetSpiInterfaceMode( GyroSpiInterfaceMode::Enum mode )
            {
                ( mValue &= ~(GyroSpiInterfaceMode::Mask) ) |= mode;
            }
    };
    
//CtrlReg5

    namespace GyroRebootMemoryContent {
    enum Enum {
         No
        ,Yes     = BITMSK(7)
    };
    }
    
    namespace GyroFifoEnable {
    enum Enum {
         No
        ,Yes     = BITMSK(6)
    };
    }
    
    namespace GyroHpfEnable {
    enum Enum {
         No
        ,Yes     = BITMSK(4)
    };
    }
    namespace GyroDataFifoFilterMode {
    enum Enum {
         Off      = 0x00
        ,HighPass = 0x01
        ,LowPass  = 0x02
    };
    }
    
    namespace GyroIntSelFilterMode {
    enum Enum {
         Off      = 0x00
        ,HighPass = 0x40
        ,LowPass  = 0x80
    };
    }
    
    class GyroCtrlReg5 
        :
        Register
    {
        public:
            GyroCtrlReg5( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
           
            void RebootMemoryContent( GyroRebootMemoryContent::Enum en )
            {
                using namespace GyroRebootMemoryContent;
                ( mValue &= ~Yes ) |= en;
            }
            
            void EnableFifo( GyroFifoEnable::Enum en)
            {
                using namespace GyroFifoEnable;
                
                ( mValue &= ~Yes ) |= en;
            }
            
            void EnableHighPassFilter( GyroHpfEnable::Enum en )
            {
                using namespace GyroHpfEnable;
                
                ( mValue &= ~Yes ) |= en;
            }
            
            void SetDataAndFifoFilterMode( GyroDataFifoFilterMode::Enum mode )
            {
                
                uint8_t Mask = 0x03;
                ( mValue &= ~(Mask) ) |= mode;
            }
            
            void SetInterruptSelectionFilterMode( GyroIntSelFilterMode::Enum mode )
            {
                uint8_t Mask = 0xC0;
                ( mValue &= ~( Mask ) ) |= mode;
            }
    };
    
//StatusReg
    
    // Bits
    namespace GyroDataAvailableAxis {
    enum Enum {
        X    = 0x01
       ,Y    = 0x02
       ,Z    = 0x04
       ,ZYX  = (Z | Y | X)
    };
    }
    namespace GyroOverrunAxis {
    enum Enum {
         X   = 0x10
        ,Y   = 0x20
        ,Z   = 0x40
        ,ZYX = (Z | Y | X)
    };
    }
    
    class GyroStatusReg
        :
        public Register
    {
        public:
            GyroStatusReg( uint8_t reg ) : Register(reg) { }
            
            using Register::GetValue;
            
            bool IsOverrunOccuring( GyroOverrunAxis::Enum axis ) const
            {
                return (mValue & axis);
            }
            
            bool IsDataAvailable( GyroDataAvailableAxis::Enum axis ) const
            {
                return (mValue & axis);
            }
    };
 
//FifoCtrlReg

    // Fifo Mode
    namespace GyroFifoMode {
    enum Enum {
         Bypass         = 0x00
        ,Fifo           = 0x20
        ,Stream         = 0x40
        ,StreamToFifo   = 0x60
        ,BypassToStream = 0x80
    };
    }
    
    class GyroFifoCtrlReg
        :
        public Register
    {
        public:
            GyroFifoCtrlReg( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
           
            void SetFifoMode( GyroFifoMode::Enum mode )
            {
                uint8_t Mask = (0x80 | 0x40 | 0x20);
                ( mValue &= ~(Mask) ) |= mode;
            }
            
            void SetFifoThreshold( uint8_t thresh )
            {
                if( thresh > 0x1F ){ thresh = 0x1F; }
                ( mValue &= ~(0x1F) ) |= thresh;
            }        
    };
    
//FifoSrcReg

    class GyroFifoSrcReg
        :
        public Register
    {
        public:
            GyroFifoSrcReg( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            bool IsSourceWatermark( void ) const
            {
                return (mValue & BITMSK(7));
            }
            
            bool IsSourceOverrun( void ) const
            {
                return (mValue & BITMSK(6));
            }
            
            bool IsFifoEmpty( void ) const
            {
                return (mValue & BITMSK(5));
            }
            
            uint8_t GetFifoLevel( void ) const
            {
                return (mValue & 0x1F);
            }
    };
    
//Int1Cfg

    // Bit Masks
    namespace GyroIntEvntCmb {
    enum Enum {
        Or   = 0x00
       ,And  = 0x80
       ,Mask = BITMSK(7)
    };
    }
    
    namespace GyroIntGen {
    enum Enum {
         XL   = 0x01
        ,XH   = 0x02
        ,YL   = 0x04
        ,YH   = 0x08
        ,ZL   = 0x10
        ,ZH   = 0x20
        ,Mask = 0x3F
    };
    }
    
    class GyroInt1CfgReg
        :
        public Register
    {
        public:
           GyroInt1CfgReg( uint8_t reg = 0 ) : Register(reg) { }

           using Register::GetValue;
           
           void SetInterruptEventCombination( GyroIntEvntCmb::Enum comb )
           {
                (mValue &= ~(GyroIntEvntCmb::Mask) ) |= comb;
           }
           
           void LatchInterruptRequest( bool isLatched )
           {
                (mValue &= ~BITMSK(6)) |= ( isLatched << 6 );
           }
           
           void SetInterruptGeneration( uint8_t intGenMask )
           {
                (mValue &= ~(GyroIntGen::Mask) ) |= (GyroIntGen::Mask & intGenMask);
           }
           
           void ClearInterruptGeneration( uint8_t intGenMask )
           {
                (mValue &= ~(GyroIntGen::Mask & intGenMask));
           }
           
           void EnableInterruptGeneration( GyroIntGen::Enum bit )
           {
                mValue |= bit;
           }
           
           void DisableInterruptGeneration( GyroIntGen::Enum bit )
           {
                mValue &= ~bit;
           }
    };
           

//Int1Src

    // Bit Masks
    namespace GyroInt1Src {
    enum Enum {
         XL   = BITMSK(0) // XL above thresh
        ,XH   = BITMSK(1) // XH above thresh
        ,YL   = BITMSK(2)
        ,YH   = BITMSK(3)
        ,ZL   = BITMSK(4)
        ,ZH   = BITMSK(5)
        ,Mask = 0x3F
    };
    }
    
    class GyroInt1SrcReg
        :
        public Register
    {
        public:
            GyroInt1SrcReg( uint8_t reg ) : Register(reg) { }
            
            using Register::GetValue;
            
            bool IsInterruptActive( void ) const
            {
                return (mValue & 0x40);
            }
            
            bool IsInterruptSource( GyroInt1Src::Enum intSrc )
            {
                return (mValue & intSrc);
            }
            
            bool IsInterruptSource( uint8_t mask )
            {
                return (mValue & (GyroInt1Src::Mask & mask) );
            }
    };

//Int1Duration

    class GyroInt1Duration
        :
        public Register
    {
        public:
            GyroInt1Duration( uint8_t reg = 0 ) : Register(reg) { }
            
            using Register::GetValue;
            
            void EnableWaitOnInterrupt( bool waitOnInt )
            {
                (mValue &= ~BITMSK(7)) |= (waitOnInt << 7);
            }
            
            void SetInterruptWaitDuration( uint8_t duration )
            {
                (mValue &= ~0x3F) |= (0x3F & duration);
            }
    };
    
#endif //__L3G4200D_GYRO_CFG_DATA_H__
