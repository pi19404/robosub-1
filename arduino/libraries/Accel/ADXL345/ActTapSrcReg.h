// Bit Masks
namespace ActSrcAxis 
{
	enum Enum 
	{
		 X = BITMSK(6)
		,Y = BITMSK(5)
		,Z = BITMSK(4)
	};
}

namespace TapSrcAxis 
{
	enum Enum 
	{
		 X = BITMSK(2)
		,Y = BITMSK(1)
		,Z = BITMSK(0)
	};
}

namespace DeviceAsleep 
{
	enum Enum 
	{
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

		bool IsDeviceAsleep();
		bool IsTapSrc( TapSrcAxis::Enum en );
		bool IsTapSrc( uint8_t mask );
		bool IsActSrc( ActSrcAxis::Enum en );
		bool IsActSrc( uint8_t mask );
};

#include "ActTapSrcReg.cpp"