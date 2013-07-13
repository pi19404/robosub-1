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
    Serial1.begin(9600);

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

    // the h bridges (SaberTooth 2x12) appear to have a startup time, and 2sec,
    // in addition to the other startup that the Arduino does, is a little too
    // fast for the first h bridge, which is for the port thrusters; a delay of
    // 4sec should be more than enough to allow ample startup of the h bridges
    delay(4000);
    _lm.LogStr("Arduino.pde sketch; initializing RoboSubController object.");
    RoboSubController Controller (ROBO_COM, 100.0);
    _lm.LogStr("Arduino.pde sketch; finished RoboSubController object.");
    _lm.LogStr("Running.");
    Controller.Run();   

    delay(3000);
}
