
#include <RealTimedPneumaticController.h>

RealTimedPneumaticController _TestController( A0, 3000 );
PneumaticControllerData      _TestData;

void setup()
{
  _TestController.AttachInputLine( _TestData );
  _TestController.Initialize();
  _TestData = true;
  _TestController.Drive();
}

void loop () { }
