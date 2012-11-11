//Config Register
namespace CompassOrientation 
{
	enum Enum 
	{
		 Level        = BITMSK(0)
		,UprightEdge  = BITMSK(1)
		,UprightFront = BITMSK(2)
	};
}

class CompassConfigReg
	:
	public Register
{
	public: 
		CompassConfigReg(uint8_t reg = 0) : Register(reg) { }

		using Register::GetValue;

		void SetOrientation( CompassOrientation::Enum orientation );
		bool IsInStandby();
		bool IsRunning();
		bool IsHeadingFilterEnabled();
		void EnableHeadingFilter( bool yesno );
		bool IsCalibrating();
		bool IsCalculatingData();
};

#include "CompassConfigReg.cpp"