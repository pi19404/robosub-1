#define DEBUG 0

#include <LogManager.h>
#include <IMCS_Configuration.h>
#include <RoboSubController.h>

#define DEVICES      _Devices
#define DEVICES_TYPE DeviceHandler*
#define NUM_DEVICES  LIST_N_ELEMS(DEVICES,DEVICES_TYPE)
#define DEVICES_LIST LIST(DEVICES_TYPE,DEVICES,DEVICE)

// MOTOR CONTROLLERS:
#define M1_ADDR 128
#define M2_ADDR 129

#define MOTOR_CTRLS      _MotorCtrls
#define MOTOR_CTRLS_TYPE RealMotorCtrlHandler*
#define NUM_MOTOR_CTRLS  LIST_N_ELEMS(MOTOR_CTRLS,MOTOR_CTRLS_TYPE)
#define MOTOR_CTRLS_LIST LIST(MOTOR_CTRLS_TYPE,MOTOR_CTRLS,MCTRL)

#include <HardwareSerial.h>
#include <SerialComPort.h>
#include <RealMotorCtrlHandler.h>

SerialComPort _M1Com( Serial1, M1_ADDR );
SerialComPort _M2Com( Serial1, M2_ADDR );

RealMotorCtrlHandler _M1( _M1Com );
RealMotorCtrlHandler _M2( _M2Com );

#define MCTRL1 LIST_FIRST_ELEM(&_M1, MCTRL2)
#define MCTRL2 LIST_ELEM(&_M2, MCTRL3 )
#define MCTRL3 LIST_END

MOTOR_CTRLS_LIST;

#define DEVICE1   LIST_FIRST_ELEM( MOTOR_CTRLS[0], DEVICE1_1 )
#define DEVICE1_1 LIST_ELEM( MOTOR_CTRLS[1], DEVICE2 )

// THRUSTERS
#include <RealThrusterPlant.h>
#include <RealThruster.h>

#define THRUSTERS      _Thrusters
#define THRUSTERS_TYPE RealThruster*
#define NUM_THRUSTERS  LIST_N_ELEMS(THRUSTERS,THRUSTERS_TYPE)
#define THRUSTERS_LIST LIST(THRUSTERS_TYPE,THRUSTERS,THRUSTER)

RealThrusterPlant THRUSTER_PLANT;

RealThruster _T1( *MOTOR_CTRLS[0], Motor::One );  // Left  forward thruster
RealThruster _T2( *MOTOR_CTRLS[1], Motor::One );  // Right forward thruster
RealThruster _T3( *MOTOR_CTRLS[0], Motor::Two );  // Front vertical thruster
RealThruster _T4( *MOTOR_CTRLS[1], Motor::Two );  // Rear  vertical thruster

#define THRUSTER1 LIST_FIRST_ELEM( &_T1, THRUSTER2 )
#define THRUSTER2 LIST_ELEM( &_T2, THRUSTER3 )
#define THRUSTER3 LIST_ELEM( &_T3, THRUSTER4 )
#define THRUSTER4 LIST_ELEM( &_T4, THRUSTER5 )
#define THRUSTER5 LIST_END

THRUSTERS_LIST;
Vector<ThrusterData,NUM_THRUSTERS> THRUSTERS_DATA;

// PNEUMATICS:
#include <RealPneumaticPlant.h>
#include <RealPneumaticController.h>
#include <RealTimedPneumaticController.h>

#define PNEUMATICS      _Pneumatics
#define PNEUMATICS_TYPE RealPneumaticController*
#define NUM_PNEUMATICS  LIST_N_ELEMS(PNEUMATICS,PNEUMATICS_TYPE)
#define PNEUMATICS_LIST LIST(PNEUMATICS_TYPE,PNEUMATICS,PNEUMATIC)

RealPneumaticPlant PNEUMATIC_PLANT;

RealTimedPneumaticController _Torpedo1 ( 52,  500 );
RealTimedPneumaticController _Torpedo2 ( 53,  500 );
RealTimedPneumaticController _Marker1  ( 49, 1500 );
RealTimedPneumaticController _Marker2  ( 31, 1500 );
RealPneumaticController      _Claw1Pos ( 41       );
RealPneumaticController      _Claw1Grip( 43       );
RealPneumaticController      _Claw2Pos ( 29       );
RealPneumaticController      _Claw2Grip( 23       );

#define PNEUMATIC1 LIST_FIRST_ELEM(&_Torpedo1, PNEUMATIC2)
#define PNEUMATIC2 LIST_ELEM(&_Torpedo2, PNEUMATIC3)
#define PNEUMATIC3 LIST_ELEM(&_Marker1,  PNEUMATIC4)
#define PNEUMATIC4 LIST_ELEM(&_Marker2,  PNEUMATIC5)
#define PNEUMATIC5 LIST_ELEM(&_Claw1Pos, PNEUMATIC6)
#define PNEUMATIC6 LIST_ELEM(&_Claw1Grip,PNEUMATIC7)
#define PNEUMATIC7 LIST_ELEM(&_Claw2Pos, PNEUMATIC8)
#define PNEUMATIC8 LIST_ELEM(&_Claw2Grip,PNEUMATIC9)
#define PNEUMATIC9 LIST_END

PNEUMATICS_LIST;
Vector<PneumaticControllerData, NUM_PNEUMATICS> PNEUMATICS_DATA;

#define DEVICE2   LIST_ELEM( PNEUMATICS[0], DEVICE2_1 )
#define DEVICE2_1 LIST_ELEM( PNEUMATICS[1], DEVICE2_2 )
#define DEVICE2_2 LIST_ELEM( PNEUMATICS[2], DEVICE2_3 )
#define DEVICE2_3 LIST_ELEM( PNEUMATICS[3], DEVICE2_4 )
#define DEVICE2_4 LIST_ELEM( PNEUMATICS[4], DEVICE2_5 )
#define DEVICE2_5 LIST_ELEM( PNEUMATICS[5], DEVICE2_6 )
#define DEVICE2_6 LIST_ELEM( PNEUMATICS[6], DEVICE2_7 )
#define DEVICE2_7 LIST_ELEM( PNEUMATICS[7], DEVICE3   )


// Leak Detection:
#define LEAK_PIN 2
#define DEVICE3   LIST_GO_TO(DEVICE4)

// Battery Monitoring
#include <AnalogDevice.h>
AnalogDevice BATT_VOLTAGE( A6 );

#define DEVICE4 LIST_ELEM(&BATT_VOLTAGE, DEVICE5)

// Compass
#define COMPASS_ADDR 0x19
#include <Wire.h>
#include <WireComPort.h>
#include <CompassData.h>
#include <RealCompassHandler.h>

WireComPort _CompassCom( Wire, COMPASS_ADDR );
RealCompassHandler COMPASS( _CompassCom );

CompassData<float> COMPASS_DATA;

//#define DEVICE5   LIST_ELEM(&COMPASS, DEVICE5_1)
#define DEVICE5 LIST_END

DEVICES_LIST;

// RoboSub Controller
#include <HardwareSerial.h>
#include <SerialComPort.h>
#include <RoboSubController.h>
#define SHORE_COM_ADDR 4
SerialComPort ROBO_COM( Serial, SHORE_COM_ADDR );

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
                                    ,THRUSTERS_DATA
                                    ,PNEUMATICS_DATA
                                    ,THRUSTER_PLANT
                                    ,PNEUMATIC_PLANT
                                    ,COMPASS_DATA
                                    ,COMPASS
                                    ,LEAK_PIN
                                    ,BATT_VOLTAGE
                                    ,100.0              // Loop at 1000Hz
                                   );
  Controller.Run();                                    
}
