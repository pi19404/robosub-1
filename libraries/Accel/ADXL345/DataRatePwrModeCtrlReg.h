// Bit Masks
namespace LowPower 
{
	enum Enum 
	{
		 No  = 0
		,Yes = BITMSK(4)
	};
}

namespace DataRate 
{
	enum Enum 
	{
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

		void EnableLowPowerMode( LowPower::Enum en );
		void SetDataRate( DataRate::Enum en );
};

#include "DataRatePwrModeCtrlReg.cpp"