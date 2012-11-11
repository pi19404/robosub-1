// Bit Masks
namespace TapAxes 
{
	enum Enum 
	{
		 None = 0
		,Z = BITMSK(0)
		,Y = BITMSK(1)
		,X = BITMSK(2)
	};
}
    
namespace DoubleTapDetection 
{
	enum Enum 
	{
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

		void EnableTapAxis( TapAxes::Enum axis );
		void DisableTapAxis( TapAxes::Enum axis );
		void SetTapAxes( uint8_t mask );
		void SetDoubleTapDetection( DoubleTapDetection::Enum en );
};

#include "TapAxesCtrlReg.cpp"
	