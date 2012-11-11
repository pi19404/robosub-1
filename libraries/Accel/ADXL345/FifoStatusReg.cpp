bool IsTriggerOccuring()
{
	return (mValue & BITMSK(7));
}

uint8_t GetFifoLevel()
{
	return (mValue & 0x3F);
}
