void TapAxesCtrlReg::EnableTapAxis( TapAxes::Enum axis )
{
	mValue |= axis;
}

void vDisableTapAxis( TapAxes::Enum axis )
{
	mValue &= ~axis;
}

void TapAxesCtrlReg::SetTapAxes( uint8_t mask )
{
	using namespace TapAxes;
	uint8_t fmask = (X | Y | Z);
	( mValue &= ~fmask ) |= (fmask & mask);
}

void TapAxesCtrlReg::SetDoubleTapDetection( DoubleTapDetection::Enum en )
{
	using namespace DoubleTapDetection;
	( mValue &= ~Yes ) |= en;
}
