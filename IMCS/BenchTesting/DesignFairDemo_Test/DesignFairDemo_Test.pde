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

RealTimedPneumaticController _Torpedo1( 53,  500 );  //SLND_1
RealTimedPneumaticController _Torpedo2( 52,  500 );  //SLND_2
RealTimedPneumaticController _Marker  ( 51, 1500 );  //SLND_3
RealPneumaticController      _ClawPos ( 50       );  //SLND_4
RealPneumaticController      _ClawGrip( 49       );  //SLND_5

#define PNEUMATIC1 LIST_FIRST_ELEM(&_Torpedo1, PNEUMATIC2)
#define PNEUMATIC2 LIST_ELEM(&_Torpedo2, PNEUMATIC3)
#define PNEUMATIC3 LIST_ELEM(&_Marker, PNEUMATIC4)
#define PNEUMATIC4 LIST_ELEM(&_ClawPos, PNEUMATIC5)
#define PNEUMATIC5 LIST_ELEM(&_ClawGrip, PNEUMATIC6)
#define PNEUMATIC6 LIST_END

PNEUMATICS_LIST;
Vector<PneumaticControllerData, NUM_PNEUMATICS> PNEUMATICS_DATA;

#define DEVICE2   LIST_ELEM( PNEUMATICS[0], DEVICE2_1 )
#define DEVICE2_1 LIST_ELEM( PNEUMATICS[1], DEVICE2_2 )
#define DEVICE2_2 LIST_ELEM( PNEUMATICS[2], DEVICE2_3 )
#define DEVICE2_3 LIST_ELEM( PNEUMATICS[3], DEVICE2_4 )
#define DEVICE2_4 LIST_ELEM( PNEUMATICS[4], DEVICE2_5 )
#define DEVICE2_5 LIST_END

DEVICES_LIST;

// RoboSub Controller
#include <Wire.h>
#include <WireComPort.h>
#include <RoboSubController.h>
#define SHORE_COM_ADDR 4

//WireComPort ROBO_COM( Wire, SHORE_COM_ADDR );
#include <SimulatedComPort.h>
#include <SimulatedDeviceController.h>

class DesignFairTest
  :
  public SimulatedDeviceController
{
  virtual void DataAvailable(SimulatedDevice &device )
  {
    // nothing to receive, just write back
    Serial.print("\n-> ");
    while( !Serial.available() );
    while( Serial.available() )
    {
      char c = Serial.read();
      Serial.write( c );
      device.WriteResponse( c );
    }    
  }  
};
DesignFairTest TEST_DEVICE;
SimulatedComPort ROBO_COM( SHORE_COM_ADDR, TEST_DEVICE );

void setup()
{
  // Startup:
  Serial.begin(9600);
  
  // Initialize Motor Controller Bus
  Serial.println("Initializing Motor Controller Bus.");
  Serial1.begin(9600);
  
  // Initialize RoboCom
  // Wire.begin();
  
  // Attach Thrusters to Plant:
  Serial.println("Attaching Thrusters to Plant.");
  for( size_t i = 0; i < NUM_THRUSTERS; ++i )
  {
      THRUSTERS[i]->AttachInputLine( THRUSTERS_DATA[i] );
      THRUSTER_PLANT.AttachThruster( *THRUSTERS[i] );
  }
  
  // Attach Pneumatics to Plant:
  Serial.println("Attaching Pneumatic Controllers to Plant.");
  for( size_t i = 0; i < NUM_PNEUMATICS; ++i )
  {
      PNEUMATICS[i]->AttachInputLine( PNEUMATICS_DATA[i] );
      PNEUMATIC_PLANT.AttachPneumaticController( *PNEUMATICS[i] );
  }
  
  // Initialize Devices
  Serial.println("Initializing Devices.");
  for( size_t i = 0; i < NUM_DEVICES; ++i )
  {
      DEVICES[i]->Initialize();
  }
}

void loop()
{
  // Zero out Thruster data and Pneumatics data
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
  Serial.println("\nRunning Demo.");
  RoboSubController Controller     (  ROBO_COM
                                    ,THRUSTERS_DATA
                                    ,PNEUMATICS_DATA
                                    ,THRUSTER_PLANT
                                    ,PNEUMATIC_PLANT
                                    ,100.0              // Loop at 100Hz
                                   );
  Controller.Run();                                    
}
