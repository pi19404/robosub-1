#ifndef __ACCEL_CFG_DATA_H__
#define __ACCEL_CFG_DATA_H__

#include <stdint.h>
#include "../Device/DeviceCfgData.h"

namespace Accel 
{
	// Register names:
	namespace RegNames 
	{
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
	static const RegStatus RegStats[] =	
	{ 
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
	#include "ActInactCtrlReg.h"

	// TapAxesCtrl
	#include "TapAxesCtrlReg.h"
		
	// ActTapSrc
	#include "ActTapSrcReg.h"
		
	// DataRateDataRatePwrModeCtrl
	#include "DataRatePwrModeCtrlReg.h"
		
	// PwrCtrl
	#include "PwrCtrlReg.h"

	// Interrupts:
	#include "Interrupts.h"
		
	// DataFormat Reg
	#include "DataFormatReg.h"

	// FifoCtrl Reg
	#include "FifoCtrlReg.h"

	// FifoStatus
	#include "FifoStatusReg.h"
}
#endif //__ACCEL_CFG_DATA_H__
