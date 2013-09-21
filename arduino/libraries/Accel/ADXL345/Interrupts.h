// Bit Masks
namespace Interrupts 
{
	enum Enum 
	{
		 None       = 0
		,Overrun    = BITMSK(0)
		,Watermark  = BITMSK(1)
		,FreeFall   = BITMSK(2)
		,Inactivity = BITMSK(3)
		,Activity   = BITMSK(4)
		,DoubleTap  = BITMSK(5)
		,SingleTap  = BITMSK(6)
		,DataReady  = BITMSK(7)
		,All        = 0xFF
	};
}

// Values
namespace InterruptMap 
{
	enum Enum 
	{
		 Int1 = 0x00
		,Int2 = 0xFF
	};
}

// IntEnable Reg
class IntCtrlReg
	:
	public Register
{
	public:
		using Register::GetValue;

		IntCtrlReg( uint8_t reg = 0 ) : Register(reg) { }

		void SetInterrupts( Interrupts::Enum en );
		void EnableInterrupt( Interrupts::Enum en );
		void DisableInterrupt( Interrupts::Enum en );
};

// IntMap Reg
class IntMapReg
	:
	public Register
{
	public:
		using Register::GetValue;

		IntMapReg( uint8_t reg = 0 ) : Register(reg) { }    

		void MapInterrupt( Interrupts::Enum en, InterruptMap::Enum map );
		void MapInterrupts( uint8_t mask, InterruptMap::Enum map );
};

// IntSrc Reg
class IntSrcReg
	:
	public Register
{
	public:
		using Register::GetValue;

		IntSrcReg( uint8_t reg = 0 ) : Register(reg) { }

		bool IsIntSource( Interrupts::Enum en )
		bool IsIntSource( uint8_t mask )
};

#include "Interrupts.cpp"