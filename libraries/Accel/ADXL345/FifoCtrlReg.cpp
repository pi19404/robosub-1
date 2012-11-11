void FifoCtrlReg::SetFifoMode( FifoMode::Enum en )
{
	( mValue &= ~FifoMode::Trigger ) |= en;
}

void FifoCtrlReg::SetTrigger( Trigger::Enum en )
{
	using namespace Trigger;  //??necessary addition??
	( mValue &= ~Int2 ) |= en;
}

void FifoCtrlReg::SetWatermarkThreshold( uint8_t threshold )
{
	( mValue &= ~0x1F ) |= (threshold & 0x1F);
}
