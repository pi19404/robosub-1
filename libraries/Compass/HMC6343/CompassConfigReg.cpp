void CompassConfigReg::SetOrientation( CompassOrientation::Enum orientation )
{
	using namespace CompassOrientation;
	uint8_t oMask = (Level | UprightEdge | UprightFront);

	( (mValue &= ~(oMask)) |= orientation );
}

bool CompassConfigReg::IsInStandby()
{
	return (mValue & BITMSK(3));
}

bool CompassConfigReg::IsRunning()
{
	return (mValue & BITMSK(4));
}

bool CompassConfigReg::IsHeadingFilterEnabled()
{
	return (mValue & BITMSK(5));
}

void CompassConfigReg::EnableHeadingFilter( bool yesno )
{
	( (mValue &= ~BITMSK(5)) |= (yesno << 5));
}

bool CompassConfigReg::IsCalibrating()
{
	return (mValue & BITMSK(6));
}

bool CompassConfigReg::IsCalculatingData()
{
	return (mValue & BITMSK(7));
}
