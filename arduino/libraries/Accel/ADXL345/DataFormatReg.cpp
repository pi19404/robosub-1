void DataFormatReg::SetTestForce( TestForce::Enum en )
{
	using namespace TestForce;
	( mValue &= ~On ) |= en;
}

void DataFormatReg::SetSpiMode( SpiMode::Enum en )
{
	using namespace SpiMode;
	( mValue &= ~_3Wire ) |= en;
}

void DataFormatReg::SetInterruptInversion( InterruptInversion::Enum en )
{
	using namespace InterruptInversion;
	( mValue &= ~Yes ) |= en;
}

void DataFormatReg::SetFullResolutionMode( FullResolutionMode::Enum en )
{
	using namespace FullResolutionMode;
	( mValue &= ~Yes ) |= en;
}

void DataFormatReg::SetJustify( Justify::Enum en )
{
	using namespace Justify;
	( mValue &= ~Left ) |= en;
}

void DataFormatReg::SetMeasurementRange( MeasurementRange::Enum en )
{
	using namespace MeasurementRange;
	( mValue &= ~_16g ) |= en;
}             
