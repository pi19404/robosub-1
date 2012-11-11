void ActInactCtrlReg::SetActivityCoupling( ActivityCoupling::Enum en )
{
	uint8_t mask = ActivityCoupling::AcCoupled;
	( mValue &= ~mask ) |= en;
}

void ActInactCtrlReg::SetInactivityCoupling( InactivityCoupling::Enum en )
{
	uint8_t mask = InactivityCoupling::AcCoupled;
	( mValue &= ~mask ) |= en;
}

void ActInactCtrlReg::EnableActivityDetection( ActivityAxis::Enum en )
{
	mValue |= en;
}

void ActInactCtrlReg::DisableActivityDetection( ActivityAxis::Enum en )
{
	mValue &= ~en;
}

void ActInactCtrlReg::EnableInactivityDetection( InactivityAxis::Enum en )
{
	mValue |= en;
}

void ActInactCtrlReg::DisableInactivityDetection( InactivityAxis::Enum en )
{
	mValue &= ~en;
}

void ActInactCtrlReg::SetActivityDetection( uint8_t mask )
{
	using namespace ActivityAxis;
	uint8_t fmask = (X | Y | Z);
	( mValue &= ~fmask ) |= (fmask & mask);
}

void ActInactCtrlReg::SetInActivityDetection( uint8_t mask )
{
	using namespace InactivityAxis;
	uint8_t fmask = (X | Y | Z);
	( mValue &= ~fmask ) |= (fmask & mask);
}