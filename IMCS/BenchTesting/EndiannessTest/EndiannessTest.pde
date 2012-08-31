#define DEBUG 0

#include <LogManager.h>
#include <IMCS_Configuration.h>

// RoboSub Controller
#include <Wire.h>
#include <HardwareSerial.h>
#include <SerialComPort.h>
#include <RoboSubControllerTest.h>
#define SHORE_COM_ADDR 4
SerialComPort ROBO_COM( Serial, SHORE_COM_ADDR );

void setup()
{
  // Startup:
  LogManager& _lm = LogManager::GetInstance();
  _lm.Begin(100);
  Serial.begin(9600);
  
  // Wait for Client
  _lm.Log("Waiting for Client.");
  delay(1000);
}

void loop()
{
  // Zero out Thruster data and Pneumatics data
  LogManager& _lm = LogManager::GetInstance();
  
  // Run Demo
  _lm.Log("\nRunning Demo.");
  RoboSubController Controller     (  
                                     ROBO_COM
                                    ,100.0              // Loop at 100Hz
                                   );
  Controller.Run();                                    
}
