// Bit Masks
namespace ActivityCoupling 
{
	enum Enum 
	{
		 DcCoupled = 0
		,AcCoupled = BITMSK(7)
	};
}
    
namespace InactivityCoupling 
{
	enum Enum 
	{
		 DcCoupled = 0
		,AcCoupled = BITMSK(3)
	};
}
    
namespace ActivityAxis 
{
	enum Enum 
	{
		 X = BITMSK(6)
		,Y = BITMSK(5)
		,Z = BITMSK(4)
	};
}
    
namespace InactivityAxis 
{
	enum Enum 
	{
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

		void SetActivityCoupling( ActivityCoupling::Enum en );
		void SetInactivityCoupling( InactivityCoupling::Enum en );
		void EnableActivityDetection( ActivityAxis::Enum en );
		void DisableActivityDetection( ActivityAxis::Enum en );
		void EnableInactivityDetection( InactivityAxis::Enum en );
		void DisableInactivityDetection( InactivityAxis::Enum en );
		void SetActivityDetection( uint8_t mask );
		void SetInActivityDetection( uint8_t mask );
};

#include "ActInactCtrlReg.cpp"