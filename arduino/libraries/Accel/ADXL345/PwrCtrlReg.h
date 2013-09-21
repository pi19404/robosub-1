// Bit Masks
namespace ActInactMode 
{
	enum Enum 
	{
		 Separate = 0
		,Linked   = BITMSK(5)
	};
}

namespace AutoSleep 
{
	enum Enum 
	{
		 No  = 0
		,Yes = BITMSK(4)
	};
}

namespace MeasurementMode 
{
	enum Enum 
	{
		 Standby = 0
		,Run     = BITMSK(3)
	};
}

namespace SleepMode 
{
	enum Enum 
	{
		StandbyRun = 0
		,Sleep      = BITMSK(2)
	};
}

namespace WakeupFrequency 
{
	enum Enum 
	{
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

		void SetActInactMode( ActInactMode::Enum en );
		void EnableAutoSleep( AutoSleep::Enum en );
		void SetMeasurementMode( MeasurementMode::Enum en );
		void SetSleepMode( SleepMode::Enum en );
		void SetWakeupFrequency( WakeupFrequency::Enum en );
};

#include "PwrCtrlReg.cpp"