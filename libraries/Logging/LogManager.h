#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include "..\Utility\Singleton.h"

#if defined (ARDUINO)
    #if (ARDUINO >= 100)
        #include <Arduino.h>
    #else
        #include <WProgram.h>
    #endif
    
    #define STRING String
    #define LOG Serial.println
    #define BEGIN(x) Serial.begin(x)
    #define DELAY(x) delay(x)
#else
    #include <string>
    #define STRING std::string
    #define LOG printf
    #define BEGIN(x)
    #define DELAY(x)
#endif
    

class LogManager
    :
    public Singleton<LogManager>
{
    public:
        LogManager() 
            : 
             Singleton<LogManager>()
            ,mDelay(0)
            ,mPrevTime(0)
        {}
    
        void Begin(unsigned rate = 0)
        {
            #if (DEBUG == 1)
            mDelay = (rate) ? (1000)/(rate) : 0;
            BEGIN(9600);
            #endif
        }
        
        void Log( const STRING& str)
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
        
    private:
       unsigned long mDelay;
       unsigned long mPrevTime;
};

#endif //__LOG_MANAGER_H__