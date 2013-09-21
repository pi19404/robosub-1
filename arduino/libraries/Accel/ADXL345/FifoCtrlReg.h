// Bit Masks
namespace FifoMode 
{
	enum Enum 
	{
		 Bypass  = 0x00
		,Fifo    = 0x40
		,Stream  = 0x80
		,Trigger = 0xC0
	};
}

namespace Trigger 
{
	enum Enum 
	{
		 Int1 = 0x00
		,Int2 = BITMSK(5)
	};
}

class FifoCtrlReg 
	:
	public Register
{
	public:
		using Register::GetValue;

		FifoCtrlReg( uint8_t reg = 0 ) : Register(reg) { }

		void SetFifoMode( FifoMode::Enum en );
		void SetTrigger( Trigger::Enum en );
		void SetWatermarkThreshold( uint8_t threshold );
};

#include "FifoCtrlReg.cpp"