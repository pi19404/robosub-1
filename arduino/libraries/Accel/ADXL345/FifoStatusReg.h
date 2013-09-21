class FifoStatusReg
	:
	public Register
{
	public:
		using Register::GetValue;

		FifoStatusReg( uint8_t reg = 0 ) : Register( reg ) { }

		bool IsTriggerOccuring();
		uint8_t GetFifoLevel();
};

#include "FifoStatusReg.cpp"