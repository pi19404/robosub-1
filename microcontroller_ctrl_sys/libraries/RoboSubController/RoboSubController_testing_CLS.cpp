#ifdef ROBOSUBCONTROLLER_TESTING_CLS

#include <RoboSubController/RoboSubController.h>
#include <Logging/LogManager.h>

RoboSubController::RoboSubController( ComPort &comPort, float loopFrequency )
    :
    mComPort(comPort)
{ 
    LogManager& mLogInstance = LogManager::GetInstance();
    mLogInstance.LogStr("RoboSubController::RoboSubController(ComPort, \
loopFrequency) constructor."); 

    m_RUN = false;
}

void RoboSubController::Run()
{
    // a generic integer for loops and stuff
    int i = 0;

    char buffer[20];

    // this is the CLS pmod that stuff can be printed to
    CLS mLittleScreen;

    // a local instance of the log manager
    LogManager& _lm = LogManager::GetInstance();

    while( true )
    {
        sprintf(buffer, "i = '%d'\n", i);
        //_lm.LogStr(buffer);
        mLittleScreen.writeStrToCLS(buffer);
        i++;
        delay(500);
    }
}

#endif  // ROBOSUBCONTROLLER_TESTING_CLS
