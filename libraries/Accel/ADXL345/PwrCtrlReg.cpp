void PwrCtrlReg::SetActInactMode( ActInactMode::Enum en )
{
	using namespace ActInactMode;
	(mValue &= ~Linked) |= en;
}

void PwrCtrlReg::EnableAutoSleep( AutoSleep::Enum en )
{
	using namespace AutoSleep;
	(mValue &= ~Yes) |= en;
}

void PwrCtrlReg::SetMeasurementMode( MeasurementMode::Enum en )
{
	using namespace MeasurementMode;
	(mValue &= ~Run) |= en;
}

void PwrCtrlReg::SetSleepMode( SleepMode::Enum en )
{
	using namespace SleepMode;
	(mValue &= ~Sleep) |= en;
}

void PwrCtrlReg::SetWakeupFrequency( WakeupFrequency::Enum en )
{
	using namespace WakeupFrequency;
	(mValue &= ~_1Hz) |= en;
}
