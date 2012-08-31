/*****************************************************************************
* Developer: Jay Sidhu
* Date: 1-Apr-2012
* Project: RoboSub
* System: MotorController Test
* SystemType: Embedded
* Description: This Program implements test cases for the Motor Controllers.
*
* Revision History: DATE:     | Description
*                   -----------------------
*                   4-01-2012  | Created.
*                   4-15-2012  | Modified to match new control system.
*****************************************************************************/
#include <IMCS_Configuration.h>

// Define Devices Macro List
#define DEVICES      _Devices
#define DEVICES_TYPE DeviceHandler*
#define NUM_DEVICES  LIST_N_ELEMS(DEVICES,DEVICES_TYPE)
#define DEVICE_LIST  LIST(DEVICES_TYPE,DEVICES,DEVICE)

///////////////////////////////////
//
//       MOTOR CONTROLLERs
//
///////////////////////////////////
#include <SerialComPort.h>
#include <RealMotorCtrlHandler.h>
#define MCTRL_COM_TYPE  SerialComPort
#define MCTRL_COM       Serial1
#define MCTRL1_ADDR     0x80
#define MCTRL2_ADDR     0x81
#define MCTRL3_ADDR     0x82
#define MCTRL_PORT_BAUD 9600

MCTRL_COM_TYPE _MotorCom1( MCTRL_COM, MCTRL1_ADDR );  // Motor Com 1
MCTRL_COM_TYPE _MotorCom2( MCTRL_COM, MCTRL2_ADDR );  // Motor Com 2
MCTRL_COM_TYPE _MotorCom3( MCTRL_COM, MCTRL3_ADDR );  // Motor Com 3

// Create MotorCtrl List
#define MOTOR_CTRLS      _MotorCtrls
#define MOTOR_CTRLS_TYPE RealMotorCtrlHandler*
#define NUM_MOTOR_CTRLS  LIST_N_ELEMS(MOTOR_CTRLS,MOTOR_CTRLS_TYPE)
#define MOTOR_CTRL_LIST  LIST(MOTOR_CTRLS_TYPE,MOTOR_CTRLS,MCTRL)

RealMotorCtrlHandler _MotorCtrl1( _MotorCom1 );  // Motor Controller 1
RealMotorCtrlHandler _MotorCtrl2( _MotorCom2 );  // Motor Controller 2
RealMotorCtrlHandler _MotorCtrl3( _MotorCom3 );  // Motor Controller 3

// Add Motor Controllers to MotorCtrl List
#define MCTRL1 LIST_FIRST_ELEM(&_MotorCtrl1, MCTRL2)
#define MCTRL2 LIST_ELEM      (&_MotorCtrl2, MCTRL3)
#define MCTRL3 LIST_ELEM      (&_MotorCtrl3, MCTRL4)
#define MCTRL4 LIST_END

// Create Motor Ctrl List
MOTOR_CTRL_LIST;

// Add Motor Controllers to Device List
#define DEVICE1   LIST_FIRST_ELEM(MOTOR_CTRLS[0], DEVICE4_1)
#define DEVICE4_1 LIST_ELEM(MOTOR_CTRLS[1], DEVICE4_2)
#define DEVICE4_2 LIST_ELEM(MOTOR_CTRLS[2], DEVICE4_3)
#define DEVICE4_3 LIST_END

// Create Device List
DEVICE_LIST;

///////////////////////////////////
//
//       THRUSTERS
//
///////////////////////////////////
#include <ThrusterData.h>
#include <RealThrusterPlant.h>
#include <RealThruster.h>

// Thruster Plant
// RealThrusterPlant _ThrusterPlant;

// Define Thruster List
#define THRUSTERS      _Thrusters
#define THRUSTERS_TYPE RealThruster
#define NUM_THRUSTERS  LIST_N_ELEMS(THRUSTERS,THRUSTERS_TYPE)
#define THRUSTER_LIST  LIST(THRUSTERS_TYPE,THRUSTERS,THRUSTER)

// Create Thrusters
#define THRUSTER1 LIST_FIRST_ELEM( THRUSTERS_TYPE( *MOTOR_CTRLS[0], Motor::One ), THRUSTER2 )  // Port-side      forward  thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[1], Motor::One ), THRUSTER3 )  // Starboard-side forward  thruster
#define THRUSTER3 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[0], Motor::Two ), THRUSTER4 )  // Bow            vertical thruster
#define THRUSTER4 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[1], Motor::Two ), THRUSTER5 )  // Stern          vertical thruster
#define THRUSTER5 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[2], Motor::One ), THRUSTER6 )  // Top            strafe   thruster
#define THRUSTER6 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[2], Motor::Two ), THRUSTER7 )  // Bottom         strafe   thruster
#define THRUSTER7 LIST_END

// End Thruster List:
THRUSTER_LIST;

// Define Thruster Inputs
#include <Vector.h>
Vector<ThrusterData, NUM_THRUSTERS> _ThrusterInputs;

const int RAMP_MAX =  90;
const int RAMP_MIN = -90;
const int RAMP_DELAY = 100;
const int STARTUP_DELAY = 1000;

void RunTests( int thruster );

void setup()
{
  // Setup Ports
  unsigned long prev, clock;
  prev = millis();
  Serial.begin(9600);
  Serial1.begin(9600);
  
  // Wait for motor controllers to boot
  Serial.println("Waiting for Motor Controllers.");
  clock = millis();
  clock -= prev;
  if( clock < STARTUP_DELAY )
  {
    delay( STARTUP_DELAY - clock );
  }
  
  // Initialize Motor Controllers
  for( int i = 0; i < NUM_DEVICES; ++i )
  {
      DEVICES[i]->Initialize();
  }
  
  // Attach IPipes
  Serial.println("Attaching Thruster Input Lines.");
  for( int i = 0; i < NUM_THRUSTERS ; ++i )
  {
    THRUSTERS[i].AttachInputLine( _ThrusterInputs[i] );
  }
  
  // Run Tests:
  char scond;
  do
  {
    Serial.print("Type 'i' To Initiate Startup Sequence: ");
    while( !Serial.available() );
    scond = Serial.read();
    Serial.println(scond);
  } while( scond != 'i' );
  
  for( int i = 0; i < NUM_THRUSTERS; ++i )
  {
    Serial.print("Running Tests for Thruster ");
    Serial.print(i+1);
    Serial.println(".");
    RunTests( i );
  }
  Serial.println("Startup Sequence Complete.");
}

void loop()
{
}


void RunTests( int thruster )
{
  unsigned long prev, clock;
  
  // Ramp up
  for( int s = 0; s < RAMP_MAX; ++s )
  {
    prev = millis();
    _ThrusterInputs[thruster] = s;
    THRUSTERS[thruster].Drive();
    clock = millis();
    clock -= prev;
    if ( clock < RAMP_DELAY )
    {
      delay( RAMP_DELAY - clock );
    }
  }
  // Ramp down
  for( int s = RAMP_MAX; s > RAMP_MIN; --s )
  {
    prev = millis();
    _ThrusterInputs[thruster] = s;
    THRUSTERS[thruster].Drive();
    clock = millis();
    clock -= prev;
    if ( clock < RAMP_DELAY )
    {
      delay( RAMP_DELAY - clock );
    }
  }
  // Return to zero
  for( int s = RAMP_MIN; s <= 0; ++s )
  {
     prev = millis();
     _ThrusterInputs[thruster] = s;
     THRUSTERS[thruster].Drive();
     clock = millis();
     clock -= prev;
     if( clock < RAMP_DELAY )
     {
        delay( RAMP_DELAY - clock );
     }
  }
}
