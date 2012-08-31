/*****************************************************************************
* Developer: Jay Sidhu
* Date: 26-Mar-2012
* Project: RoboSub
* System: Inertial Measurement and Control System
* SystemType: Embedded
* Description: This Program implements the Inertial measurement and control 
*              system for the vehicle.
*
* Revision History: DATE:     | Description
*                   -----------------------
*                   3-26-2012 | Created.
*                   3-29-2012 | Added Skeleton Code for Devices / Controllers
*                   3-30-2012 | Added SerialComPort / RealMotorCtrlHandler(s)
*                   3-31-2012 | Build 0: No Accel, No Gyro
*                   4-08-2012 | Build 1: 1st build after major code refactor
*                               Added Thruster/Thruster Plant, TimeOut on
*                               ComPorts when Device is not attached.
*                   4-11-2012 | Build 2: Reworked design, added Pipeline model
*                               for data synchronization across subsystems.
*                               Added Gyro. Added Configuration File.
*                               Added Heading Controller, and Kp values for
*                               Heading and Balance(Pitch/Roll) controllers.
*                   4-12-2012 | Added LIST macro tools for configurable static
*                               allocation of objects.
*                   4-14-2012 | Added Accelerometer Handler.
*****************************************************************************/

#include <IMCS_Configuration.h>  // Configuration File
#include <LogManager.h>          // LogManager

// Define Devices Macro List
#define DEVICES      _Devices
#define DEVICES_TYPE DeviceHandler*
#define NUM_DEVICES  LIST_N_ELEMS(DEVICES,DEVICES_TYPE)
#define DEVICE_LIST  LIST(DEVICES_TYPE,DEVICES,DEVICE)



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

// Add Compass to Devices List
#define DEVICE1 LIST_FIRST_ELEM(&_Compass,DEVICE2)

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

// Add Gyro to Devices List
#define DEVICE2 LIST_ELEM(&_Gyro,DEVICE3)

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

// Add Acl to Devices List (Skip for now)
#define DEVICE3 LIST_ELEM(&_Accel, DEVICE4)

///////////////////////////////////
//
//       MOTOR CONTROLLERs
//
///////////////////////////////////
#include <HardwareSerial.h>
#include <SerialComPort.h>
#include <RealMotorCtrlHandler.h>
#define MCTRL_COM_BUS   Serial1
#define MCTRL_COM_TYPE  SerialComPort
#define MCTRL1_ADDR     0x80
#define MCTRL2_ADDR     0x81
#define MCTRL3_ADDR     0x82
#define MCTRL_PORT_BAUD 9600

MCTRL_COM_TYPE _MotorCom1( MCTRL_COM_BUS, MCTRL1_ADDR );  // Motor Com 1
MCTRL_COM_TYPE _MotorCom2( MCTRL_COM_BUS, MCTRL2_ADDR );  // Motor Com 2
MCTRL_COM_TYPE _MotorCom3( MCTRL_COM_BUS, MCTRL3_ADDR );  // Motor Com 3

// Create MotorCtrl List
#define MOTOR_CTRLS      _MotorCtrls
#define MOTOR_CTRL_TYPE  RealMotorCtrlHandler
#define MOTOR_CTRLS_TYPE MOTOR_CTRL_TYPE*
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
#define DEVICE4   LIST_ELEM(MOTOR_CTRLS[0], DEVICE4_1)
#define DEVICE4_1 LIST_ELEM(MOTOR_CTRLS[1], DEVICE4_2)
#define DEVICE4_2 LIST_ELEM(MOTOR_CTRLS[2], DEVICE4_3)
#define DEVICE4_3 LIST_END

// Create Device List
DEVICE_LIST;

//////////////////////////////////////////
//
//       CONTROL SYSTEM (CONTROLLERS)
//
//////////////////////////////////////////
#include <PIController.h>

#define HDG_KP 0.705  // Actual Kp value for Heading Controller (Heading is +/- 180 degrees).
#define HDG_KI 0.000  // dummy value right now, Heading Controller Ki value
#define BAL_KP 1.411  // Actual Kp value for Balancing (Pitch,Roll) Controller (Pitch,Roll values are +/- 90 degrees).
#define BAL_KI 0.000  // dummy value right now, Balancing Controller Ki value
#define DIS_KP 1.000  // dummy value right now, displacement controller Kp value
#define DIS_KI 0.000  // dummy value right now, displacement controller Ki value

// Define Controller
#define CONTROLLERS      _Controllers
#define CONTROLLER_TYPE  PIController<float,int8_t>
#define CONTROLLERS_TYPE CONTROLLER_TYPE*
#define NUM_CONTROLLERS  LIST_N_ELEMS(CONTROLLERS,CONTROLLERS_TYPE)
#define CONTROLLER_LIST  LIST(CONTROLLERS_TYPE,CONTROLLERS,CONTROLLER)

CONTROLLER_TYPE _HeadingController     ( HDG_KP, HDG_KI ); // Heading Controller      (Yaw) +/- 180
CONTROLLER_TYPE _BalanceController     ( BAL_KP, BAL_KI ); // Balance Controller      (Pitch/Roll) +/- 90
CONTROLLER_TYPE _DisplacementController( DIS_KP, DIS_KI ); // Displacement Controller (X,Y,Z) in feet 

#define CONTROLLER1 LIST_FIRST_ELEM(&_HeadingController     , CONTROLLER2)
#define CONTROLLER2 LIST_ELEM      (&_BalanceController     , CONTROLLER3)
#define CONTROLLER3 LIST_ELEM      (&_DisplacementController, CONTROLLER4)
#define CONTROLLER4 LIST_END

// Create the List:
CONTROLLER_LIST;

///////////////////////////////////
//
//       THRUSTERS
//
///////////////////////////////////
#include <RealThrusterPlant.h>
#include <RealThruster.h>

// Thruster Plant
RealThrusterPlant _ThrusterPlant;

// Define Thruster List
#define THRUSTERS      _Thrusters
#define THRUSTERS_TYPE RealThruster
#define NUM_THRUSTERS  LIST_N_ELEMS(THRUSTERS,THRUSTERS_TYPE)
#define THRUSTER_LIST  LIST(THRUSTERS_TYPE,THRUSTERS,THRUSTER)

// Create Thrusters
#define THRUSTER1 LIST_FIRST_ELEM( THRUSTERS_TYPE( *MOTOR_CTRLS[0], Motor::One ), THRUSTER2 )  // Port-side      forward  thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[1], Motor::Two ), THRUSTER3 )  // Starboard-side forward  thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[0], Motor::One ), THRUSTER4 )  // Bow            vertical thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[1], Motor::Two ), THRUSTER5 )  // Stern          vertical thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[2], Motor::One ), THRUSTER6 )  // Top            strafe   thruster
#define THRUSTER2 LIST_ELEM      ( THRUSTERS_TYPE( *MOTOR_CTRLS[2], Motor::Two ), THRUSTER7 )  // Bottom         strafe   thruster
#define THRUSTER7 LIST_END

// End Thruster List:
THRUSTER_LIST;

// Define Thruster Inputs
Vector<int8_t, NUM_THRUSTERS> _ThrusterInputs;

void setup()
{
  
  // Get log manager
  LogManager& _lm = LogManager::GetInstance();
  
  // Initialize Debugging
  _lm.Begin();
  _lm.Log("Initializing Inertial Measurement and Control System:");
  
  // Join the I2C bus for Wire devices
  _lm.Log("--Initializing I2C Bus.");
  Wire.begin();
  
  // Initialize the Motor Controller Serial bus
  _lm.Log("--Initializing Serial1 Bus.");
  Serial1.begin(MCTRL_PORT_BAUD);
  
  // Attach Thrusters to Thruster Plant
  _lm.Log("--Attaching Thrusters to Plant.");
  for( unsigned i = 0; i < NUM_THRUSTERS; ++i )
  {
    THRUSTERS[i].AttachInputLine( _ThrusterInputs[i] );
    _ThrusterPlant.AttachThruster( THRUSTERS[i] );
  }
  
  // Create Pipelines
  _lm.Log("--Creating Data Pipelines"); 
  /////////////////////////////////
  //
  //    TODO: Create Pipelines
  //
  /////////////////////////////////
  
  // Initialize Devices
  _lm.Log("--Initializing Device Handlers.");
  for( size_t i = 0; i < NUM_DEVICES; ++i )
  {
      DEVICES[i]->Initialize();
  }
  _lm.Log("IMCS Ready.");
}

void loop()
{
  
}
    
    


