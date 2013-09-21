// Rate Register  
class CompassRateReg
	:
	public Register
{
	public:
		enum Enum 
		{
			 _1Hz  = 0b00
			,_5Hz  = 0b01
			,_10Hz = 0b10
		};

		CompassRateReg( uint8_t reg = 0 ) : Register(reg) { }

		using Register::GetValue;

		Enum GetDeviceRate();
		void SetDeviceRate( Enum rate );
};

#include "CompassRateReg.cpp"