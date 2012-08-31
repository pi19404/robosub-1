#include <IMCS_Configuration.h>
#include <LogManager.h>

#define DEVICES      _Devices
#define DEVICES_TYPE MeasurementDevice*
#define NUM_DEVICES  LIST_N_ELEMS(DEVICES, DEVICES_TYPE)
#define DEVICE_LIST  LIST(DEVICES_TYPE, DEVICES, DEVICE)

///////////////////////////////////
//
//       COMPASS
//
///////////////////////////////////
#include <Wire.h>
#include <WireComPort.h>
#include <RealCompassHandler.h>
#define COMPASS_COM_BUS Wire
#define COMPASS_ADDR    0x19

WireComPort        _CompassCom( COMPASS_COM_BUS, COMPASS_ADDR );
RealCompassHandler _Compass( _CompassCom );
CompassData<float> _CompassData;

#define DEVICE1 LIST_FIRST_ELEM(&_Compass, DEVICE2)

#if 0 //DISABLE GYRO ACL
///////////////////////////////////
//
//       GYRO
//
///////////////////////////////////
#include <Wire.h>
#include <WireComPort.h>
#include <RealGyroHandler.h>
#define GYRO_COM_BUS Wire
#define GYRO_ADDR    0x69

WireComPort     _GyroCom( GYRO_COM_BUS, GYRO_ADDR );
RealGyroHandler _Gyro( _GyroCom );
GyroData<float> _GyroData;

#define DEVICE2 LIST_ELEM(&_Gyro, DEVICE3)

///////////////////////////////////
//
//       ACL
//
///////////////////////////////////
#include <Wire.h>
#include <WireComPort.h>
#include <RealAccelHandler.h>
#define ACCEL_COM_BUS Wire
#define ACCEL_ADDR1   0x1D

WireComPort      _AccelCom( ACCEL_COM_BUS, ACCEL_ADDR1 );
RealAccelHandler _Accel( _AccelCom );
AccelData<float> _AccelData;

// Add Acl to Devices List (Skip for now)
#define DEVICE3 LIST_ELEM(&_Accel, DEVICE4)

#endif //DISABLE GRYO ACL

#define DEVICE2 LIST_END

DEVICE_LIST;

void printData();
unsigned long prevTime, curTime;

#define LOOP_DELAY 500
void setup()
{
  LogManager& _lm = LogManager::GetInstance();
  _lm.Begin(); Serial.begin(9600);
  _lm.Log("Booting Devices Bench Test Program.");
  
  // Join I2C Bus
  _lm.Log("--Joining I2C Bus for Wire Devices.");
  Wire.begin();
  
  // Create Data Bus
  _lm.Log("--Joining Devices to Data Piplelines.");
  _Compass.AttachOutputLine( _CompassData );
  //_Gyro.AttachOutputLine( _GyroData );
  //_Accel.AttachOutputLine( _AccelData );
  
  // Initialize Devices
  _lm.Log("--Initializing Device Handlers.");
  for( size_t i = 0; i < NUM_DEVICES; ++i )
  {
      DEVICES[i]->Initialize();
  }
  _lm.Log("Ready. Waiting for Devices to boot.");
  delay(1500);  
  
  prevTime = millis();
  curTime = millis();
}

void loop()
{
  // Get Data
  for( size_t i = 0; i < NUM_DEVICES; ++i )
  {
    DEVICES[i]->GetData();
  }
  
  // Print Data
  printData();
  
  curTime = millis();
  if( (curTime - prevTime) < LOOP_DELAY )
  {
    delay( LOOP_DELAY-(curTime - prevTime) );
  }
  prevTime = millis();;
}

void printData()
{
    // Compass Data:
    Serial.print("Compass: <");
    for( int i = 0; i < 3; ++i )
    {
      Serial.print(_CompassData[i]);
      Serial.print( (i < 2) ? ", " : "");
    }
    #if 0
    Serial.print(">");
    
    // Gyro Data
    Serial.print(" Gyro: <");
    for( int i = 0; i < 3; ++i )
    {
      Serial.print(_GyroData[i]);
      Serial.print( (i < 2) ? ", " : "");
    }
    Serial.print(">");
    
    // Accel Data
    Serial.print(" Accel: <");
    for( int i = 0; i < 3; ++i )
    {
      Serial.print(_AccelData[i]);
      Serial.print( (i < 2) ? ", " : "");
    }
    #endif
    Serial.println(">");
}

