// IntCtrlReg
void IntCtrlReg::SetInterrupts( Interrupts::Enum en )
{
	( mValue &= ~0xFF ) |= en;
}

void IntCtrlReg::EnableInterrupt( Interrupts::Enum en )
{
	mValue |= en;
}

void IntCtrlReg::DisableInterrupt( Interrupts::Enum en )
{
	mValue &= ~en;
}

// IntMapReg
void IntMapReg::MapInterrupt( Interrupts::Enum en, InterruptMap::Enum map )
{
	( mValue &= ~en ) |= (en & map);
}

void IntMapReg::MapInterrupts( uint8_t mask, InterruptMap::Enum map )
{
	( mValue &= ~0xFF ) |= (map & mask);
}

// IntSrcReg
bool IntSrcReg::IsIntSource( Interrupts::Enum en )
{
	return (mValue & en);
}

bool IntSrcReg::IsIntSource( uint8_t mask )
{
	return (mValue & mask);
}
