#include "LogManager.h"

void LogManager::Begin(unsigned rate = 0)
{
	#if (DEBUG == 1)
	mDelay = (rate) ? (1000)/(rate) : 0;
	BEGIN(9600);
	#endif
}

void LogManager::Log(const STRING& str)
{
	#if (DEBUG == 1)
	unsigned long curTime = millis();
	if( !mPrevTime )
	{
		mPrevTime = curTime;
	}
	else if( (curTime - mPrevTime) < mDelay )
	{
		DELAY( mDelay - (curTime - mPrevTime) );
	}

	LOG(str);

	mPrevTime = curTime;
	#endif
}