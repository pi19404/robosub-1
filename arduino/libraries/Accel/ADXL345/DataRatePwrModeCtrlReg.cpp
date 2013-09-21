void DataRatePwrModeCtrlReg::EnableLowPowerMode( LowPower::Enum en )
{
	using namespace LowPower;
	( mValue &= ~Yes ) |= en;
}

void DataRatePwrModeCtrlReg::SetDataRate( DataRate::Enum en )
{
	uint8_t mask = 0x0F;
	( mValue &= ~mask ) |= en;
}
