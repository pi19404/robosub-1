#define DEBUG 0

#include <Logging/LogManager.h>
#include <Configurations/IMCS_Configuration.h>

// RoboSub Controller
#include <Wire.h>
#include <ComPort/SerialComPort.h>
#include <RoboSubController/RoboSubController.h>
#define SHORE_COM_ADDR 4
SerialComPort ROBO_COM( Serial, SHORE_COM_ADDR );

void beginInterrupt()
{
  if( digitalRead(RUN_PIN) )
  {
    _RUN = true;
  }
  else
  {
    _RUN = false;
  }
}

void setup()
{
  // Startup:
  LogManager& _lm = LogManager::GetInstance();
  _lm.Begin(100);
  Serial.begin(9600);
  
  // Initialize Motor Controller Bus
  _lm.Log("Initializing Motor Controller Bus.");
  Serial1.begin(9600);
  
  // Initialize I2C Bus
  _lm.Log("Initializing I2C Bus for Wire Devices.");
  Wire.begin();
  
  // Attach Compass Pipeline
  _lm.Log("Attaching Compass.");
  COMPASS.AttachOutputLine( COMPASS_DATA );
  
  // Attach Thrusters to Plant:
  _lm.Log("Attaching Thrusters to Plant.");
  for( size_t i = 0; i < NUM_THRUSTERS; ++i )
  {
      THRUSTERS[i]->AttachInputLine( THRUSTERS_DATA[i] );
      THRUSTER_PLANT.AttachThruster( *THRUSTERS[i] );
  }
  
  // Attach Pneumatics to Plant:
  _lm.Log("Attaching Pneumatic Controllers to Plant.");
  for( size_t i = 0; i < NUM_PNEUMATICS; ++i )
  {
      PNEUMATICS[i]->AttachInputLine( PNEUMATICS_DATA[i] );
      PNEUMATIC_PLANT.AttachPneumaticController( *PNEUMATICS[i] );
  }
  
  // Initialize Devices
  _lm.Log("Initializing Devices.");
  for( size_t i = 0; i < NUM_DEVICES; ++i )
  {
      DEVICES[i]->Initialize();
  }
  
  // Attach Interrupt handler
  //RUN_INTERRUPT.SetHandler( beginInterrupt );
  
  // Initialize _RUN
  //_RUN = digitalRead(RUN_PIN);
  
  // Wait for Client
  _lm.Log("Waiting for Client.");
  delay(1000);
}

void loop()
{
  // Zero out Thruster data and Pneumatics data
  LogManager& _lm = LogManager::GetInstance();
  for( size_t i = 0; i < NUM_THRUSTERS; ++i )
  {
      THRUSTERS_DATA[i] = 0;
  }
  for( size_t i = 0; i < NUM_PNEUMATICS; ++i )
  {
      PNEUMATICS_DATA[i] = 0;
  }
  THRUSTER_PLANT.DriveAll();
  PNEUMATIC_PLANT.CommandAll();
  
  // Run Demo
  _lm.Log("\nRunning Demo.");
  RoboSubController Controller     (  
                                     ROBO_COM
                                    ,100.0              // Loop at 100Hz
                                   );
  Controller.Run();                                    
}
