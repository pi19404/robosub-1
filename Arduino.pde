//#define DEBUG 1

#include <Logging/LogManager.h>

// RoboSub Controller
#include <Wire.h>
#include <ComPort/SerialComPort.h>
#include <RoboSubController/RoboSubController.h>
#define SHORE_COM_ADDR 4
SerialComPort ROBO_COM( Serial, SHORE_COM_ADDR );

void beginInterrupt()
{
}

void setup()
{
    // Startup:
    LogManager& _lm = LogManager::GetInstance();
    _lm.Begin(100);
    Serial.begin(115200);

    _lm.LogStr("Arduino.pde sketch; begin setup.");

    // Initialize Motor Controller Bus
    _lm.LogStr("Initializing Motor Controller Bus.");
    Serial1.begin(115200);

    // Initialize I2C Bus
    _lm.LogStr("Initializing I2C Bus for Wire Devices.");
    Wire.begin();

    // Initialize _RUN
    //_RUN = digitalRead(RUN_PIN);

    // Wait for Client
    _lm.LogStr("Waiting for Client.");
}

void loop()
{
    LogManager& _lm = LogManager::GetInstance();
    _lm.LogStr("\nArduino.pde sketch; begin loop.");

    // Loop at 100Hz
    delay(2000);
    _lm.LogStr("Arduino.pde sketch; initializing RoboSubController object.");
    RoboSubController Controller (ROBO_COM, 100.0);
    _lm.LogStr("Arduino.pde sketch; finished RoboSubController object.");
    _lm.LogStr("Running.");
    Controller.Run();   

    delay(3000);
}
