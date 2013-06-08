#ifdef LOGGING

#include "LogManager.h"

void LogManager::Begin(unsigned rate = 0)
{
    #if (LOGGING == 1)
    mDelay = (rate) ? (1000)/(rate) : 0;
    BEGIN(9600);
    #endif
}

void LogManager::LogStr( const STRING& str)
{
    #if (LOGGING == 1)
    unsigned long curTime = millis();
    if( !mPrevTime )
    {
        mPrevTime = curTime;
    }
    else if( (curTime - mPrevTime) < mDelay )
    {
        DELAY( mDelay - (curTime - mPrevTime) );
    }

    Serial.print(str + String("\n"));

    mPrevTime = curTime;
    #endif
}

void LogManager::LogStrInt(const STRING& str, int i)
{
    #if (LOGGING == 1)
    unsigned long curTime = millis();
    if( !mPrevTime )
    {
        mPrevTime = curTime;
    }
    else if( (curTime - mPrevTime) < mDelay )
    {
        DELAY( mDelay - (curTime - mPrevTime) );
    }

    Serial.print(str + String(i, DEC) + String("\n"));

    mPrevTime = curTime;
    #endif
}

void LogManager::LogStrHex(const STRING& str, unsigned int x)
{
    #if (LOGGING == 1)
    unsigned long curTime = millis();
    if( !mPrevTime )
    {
        mPrevTime = curTime;
    }
    else if( (curTime - mPrevTime) < mDelay )
    {
        DELAY( mDelay - (curTime - mPrevTime) );
    }

    Serial.print(str + String(x, HEX) + String("\n"));

    mPrevTime = curTime;
    #endif
}

void LogManager::LogStrFloat(const STRING& str, float fl)
{
    #if (LOGGING == 1)
    unsigned long curTime = millis();
    if( !mPrevTime )
    {
        mPrevTime = curTime;
    }
    else if( (curTime - mPrevTime) < mDelay )
    {
        DELAY( mDelay - (curTime - mPrevTime) );
    }

    Serial.print(str);
    Serial.println(fl, 2);  // this function prints its own '\n' character

    mPrevTime = curTime;
    #endif
}

#endif
