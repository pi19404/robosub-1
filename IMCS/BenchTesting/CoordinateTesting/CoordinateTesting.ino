#include <DeviceHandlerFactory.h>
#include <RealCompassHandler.h>
#include <BasisVector.h>
#include <Wire.h>
#include <WireComPort.h>

#define COMPASS_ADDRESS 0x19

using namespace Compass;

// Compass Is Attached via TWI
WireComPort _CompassComPort( Wire, COMPASS_ADDRESS );

// Get The Create() method for a real compass: (this is a ptr to a static method of DeviceHandlerFactory)
RealCompassHandler& (*_CreateRealCompassPtr)( ComPort & ) = &DeviceHandlerFactory::CreateRealDeviceHandler<RealCompassHandler>;

// Create a real compass device handler on the TWI com port
RealCompassHandler& _Compass = (*_CreateRealCompassPtr)( _CompassComPort );

void setup()
{
  Serial.begin(9600);
  // Uncomment the following two lines to enable to the real compass
  Wire.begin(); // Join the I2C Bus
  _Compass.Initialize();
  
  BasisVector basisVector = _Compass.GetBasisVector();
  
  Serial.println("Vehicle Coords to Compass Coords:");
  Serial.println("Vehicle  +X: +Y: +Z:");
  Serial.print  ("Compass  ");
  
  for( int i = 0; i < basisVector.GetSize(); ++i )
  {
    int coord = basisVector[i];
    if( coord < 0 )
    {
       Serial.print("-");
       coord = -coord;
    }
    else
    {
       Serial.print("+");
    }
    
    switch( coord )
    {
      case 1: Serial.print("X"); break;
      case 2: Serial.print("Y"); break;
      case 3: Serial.print("Z"); break;
      default: ;;
    }
    Serial.print("  ");  
  }
  Serial.println("");
}

void loop()
{
  
  
}

