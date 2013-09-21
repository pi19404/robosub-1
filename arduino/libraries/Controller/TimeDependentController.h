#ifndef __TIME_DEPENDENT_CONTROLLER_H__
#define __TIME_DEPENDENT_CONTROLLER_H__

#include "AbstractController.h"

class TimeDependentController
    :
     public AbstractController  
{
    public:
    
        TimeDependentController() 
            :
             AbstractController()
            ,mTimeStamp(0) { }
        
        // from AbstractController 
        virtual void Compute() = 0;
        
        // Reset the timestamp
        virtual void Reset(){ mTimeStamp = 0; }
        
    protected:
        unsigned long TimeStamp(){ return mTimeStamp; }
        void TimeStamp( unsigned long ts ) { mTimeStamp = ts; }
        
    private:
        unsigned long mTimeStamp;
};
        
#endif //__TIME_DEPENDENT_CONTROLLER_H__
