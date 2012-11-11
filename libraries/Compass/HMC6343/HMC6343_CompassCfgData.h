#ifndef __HMC6343_COMPASS_CFG_DATA_H__
#define __HMC6343_COMPASS_CFG_DATA_H__

#include "../../Device/DeviceCfgData.h"
#include <stdint.h>

namespace CompassRegNames 
{
	enum Enum 
	{
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

namespace CompassEepromRegNames 
{
	enum Enum 
	{
		 Config
		,Rate
		,HeadingFilter
	};
}

const static RegStatus CompassEepromStats [] = 
{
	 { 0x04, RegMode::ReadWrite }
	,{ 0x05, RegMode::ReadWrite }
	,{ 0x14, RegMode::ReadWrite }
};

const static RegStatus CompassRegStats [] = 
{
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

// EEPROM Registers
#include "CompassConfigReg.h"
#include "CompassRateReg.h"


#endif //__HMC6343_COMPASS_CFG_DATA_H__
