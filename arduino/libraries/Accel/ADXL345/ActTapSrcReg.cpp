bool ActTapSrcReg::IsDeviceAsleep()
{
	return (mValue & DeviceAsleep::Yes);
}

bool ActTapSrcReg::IsTapSrc( TapSrcAxis::Enum en )
{
	return (mValue & en);
}

bool ActTapSrcReg::IsTapSrc( uint8_t mask )
{
	using namespace TapSrcAxis;
	uint8_t fmask = (X | Y | Z);
	return (mValue & (fmask & mask) );
}

bool ActTapSrcReg::IsActSrc( ActSrcAxis::Enum en )
{
	return (mValue & en);
}

bool ActTapSrcReg::IsActSrc( uint8_t mask )
{
	using namespace ActSrcAxis;
	uint8_t fmask = (X | Y | Z);
	return (mValue & (fmask & mask));
}
