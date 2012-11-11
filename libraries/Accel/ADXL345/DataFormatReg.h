// Bit Masks
namespace TestForce 
{
	enum Enum 
	{
		 Off = 0
		,On  = BITMSK(7)
	};
}

namespace SpiMode 
{
	enum Enum 
	{
		 _4Wire = 0
		,_3Wire = BITMSK(6)
	};
}

namespace InterruptInversion 
{
	enum Enum 
	{
		 No  = 0
		,Yes = BITMSK(5)
	};
}

namespace FullResolutionMode 
{
	enum Enum 
	{
		 No  = 0
		,Yes = BITMSK(3)
	};
}

namespace Justify 
{
	enum Enum 
	{
		 Right = 0
		,Left  = BITMSK(2)  // MSB first
	};
}

namespace MeasurementRange 
{
	enum Enum 
	{
		 _2g    // +/- 2g 
		,_4g
		,_8g
		,_16g
	};
}

class DataFormatReg 
	:
	public Register
{
	public:
		using Register::GetValue;

		DataFormatReg( uint8_t reg = 0 ) : Register(reg) { }

		void SetTestForce( TestForce::Enum en );
		void SetSpiMode( SpiMode::Enum en );
		void SetInterruptInversion( InterruptInversion::Enum en );
		void SetFullResolutionMode( FullResolutionMode::Enum en );
		void SetJustify( Justify::Enum en );
		void SetMeasurementRange( MeasurementRange::Enum en );
};

#include "DataFormatReg.cpp"