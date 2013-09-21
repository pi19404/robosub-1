Enum CompassRateReg::GetDeviceRate()
{
	uint8_t rMask = (_5Hz | _10Hz);

	return (Enum)(mValue & rMask);
}

void CompassRateReg::SetDeviceRate( Enum rate )
{
	uint8_t rMask = (_5Hz | _10Hz);

	( (mValue &= ~(rMask)) |= rate );
}
