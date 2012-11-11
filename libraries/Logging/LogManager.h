#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include "../Utility/Singleton.h"

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
    
        void Begin(unsigned rate);
        void Log( const STRING& str);
        
    private:
       unsigned long mDelay;
       unsigned long mPrevTime;
};

#endif //__LOG_MANAGER_H__
