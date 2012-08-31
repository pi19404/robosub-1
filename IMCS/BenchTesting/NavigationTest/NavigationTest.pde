/*****************************************************************************
* Developer: Jay Sidhu
* Date: 26-Mar-2012
* Project: RoboSub
* System: Inertial Measurement and Control System
* SystemType: Embedded
* Description: This Program implements the Navigation Test Program for the
*              RoboSub.
* IMCS Build: 4/8/2012
*
*****************************************************************************/
#include <new.h>

// Controller Defintions:
#define BAL_KP 1.000  // dummy value right now, balance controller kp value
#define BAL_KI 0.000  // dummy value right now, balance controller ki value
#define LIN_KP 1.000  // dummy value right now, linear controller kp value
#define LIN_KI 0.000  // dummy value right now, linear controller ki value

// Port Definitions:
#define COMPASS_ADDR    0x19
#define GYRO_ADDR       0x69
#define ACL_ADDR        0x1D
#define MCTRL1_ADDR     0x80
#define MCTRL2_ADDR     0x81
#define MCTRL3_ADDR     0x82
#define MCTRL_PORT_BAUD 9600

// Log Manager Include
#include <LogManager.h>

// Ports Include
#include <Wire.h>
#include <HardwareSerial.h>
#include <WireComPort.h>
#include <SerialComPort.h>

// Devices Include
#include <RealCompassHandler.h>
//#include <RealGyroHandler.h>
//#include <RealAclHandler.h>
#include <RealMotorCtrlHandler.h>

// Control System Include
#include <PIController.h>
#include <RealThruster.h>
#include <RealThrusterPlant.h>

// Create Com Ports
WireComPort _CompassCom( Wire, COMPASS_ADDR );
//WireComPort _GyroCom( Wire, GYRO_ADDR );
//WireComPort _AclCom( Wire, ACL_ADDR );
SerialComPort _CtrlCom1( Serial1, MCTRL1_ADDR );
SerialComPort _CtrlCom2( Serial1, MCTRL2_ADDR );
SerialComPort _CtrlCom3( Serial1, MCTRL3_ADDR );

// Create Com Devices
RealCompassHandler _Compass( _CompassCom );
//RealGyroHandler Gyro   ( _GyroCom );
//RealAclHandler _Acl    ( _AclCom  );
RealMotorCtrlHandler _Controller1( _CtrlCom1 );
RealMotorCtrlHandler _Controller2( _CtrlCom2 );
RealMotorCtrlHandler _Controller3( _CtrlCom3 );

DeviceHandler* const _DeviceHandlers[] = {
                                           &_Compass
                                          //,&_Gryo
                                          //,&_Acl
                                          ,&_Controller1
                                          ,&_Controller2
                                          ,&_Controller3
};
const unsigned NUM_DEVICE_HANDLERS = (sizeof(_DeviceHandlers)/sizeof(DeviceHandler*));

// Create Controllers
PIController<float> _BalancingController( BAL_KP, BAL_KI );
PIController<float> _LinearController( LIN_KP, LIN_KI );

float _BalanceDataIn[] = {
                          0.0  // Yaw
                         ,0.0  // Pitch
                         ,0.0  // Roll
};
float _BalanceDataOut[] = {
                          0.0  // Yaw
                         ,0.0  // Pitch
                         ,0.0  // Roll
};
unsigned const NUM_BAL_PARAMS = (sizeof(_BalanceDataIn)/sizeof(float));

TiltData<float> _TiltData;   // Tilt data from Compass

unsigned const NUM_LIN_PARAMS = (sizeof(_LinearDataIn)/sizeof(float));

// Create Thrusters
RealThrusterPlant _ThrusterPlant;
RealThruster _Thruster1( _Controller1, Motor::One );  // Port-side forward thruster
RealThruster _Thruster2( _Controller2, Motor::One );  // Starboard-side forward thruster
RealThruster _Thruster3( _Controller1, Motor::Two );  // Bow vertical thruster
RealThruster _Thruster4( _Controller2, Motor::Two );  // Stern vertical thruster
RealThruster _Thruster5( _Controller3, Motor::One );  // Top-side strafe thruster
RealThruster _Thruster6( _Controller3, Motor::Two );  // Bottom-side strafe thruster

Thruster* _Thrusters[] = {
                                  &_Thruster1
                                 ,&_Thruster2
                                 ,&_Thruster3
                                 ,&_Thruster4
                                 ,&_Thruster5
                                 ,&_Thruster6
};
int8_t _ThrusterInputs[] = { 
                             0   // Thruster 1 Input Source
                            ,0   // Thruster 2 Input Source
                            ,0   // Thruster 3 Input Source
                            ,0   // Thruster 4 Input Source
                            ,0   // Thruster 5 Input Source
                            ,0   // Thruster 6 Input Source
};

const unsigned NUM_THRUSTERS = (sizeof(_Thrusters)/sizeof(Thruster*));

void setup()
{
  // Get log manager
  LogManager& _lm = LogManager::GetInstance();
  
  // Initialize Debugging
  _lm.Begin(9600);
  _lm.Log("Initializing Inertial Measurement and Control System:");
  
  // Join the I2C bus for Wire devices
  _lm.Log("--Initializing I2C Bus.");
  Wire.begin();
  
  // Initialize the Motor Controller Serial bus
  _lm.Log("--Initializing Serial1 Bus.");
  Serial1.begin(MCTRL_PORT_BAUD);
  
  // Attach Thrusters to Thruster Plant
  _lm.Log("--Attaching Thrusters.");
  for( unsigned i = 0; i < NUM_THRUSTERS; ++i )
  {
    _ThrusterPlant.AttachThruster( *_Thrusters[i], _ThrusterInputs[i] );
  }
  
  // Attach Balancing Controller
  _lm.Log("--Attaching Balancing System Controller.");
  for( unsigned i = 0; i < NUM_BAL_PARAMS; ++i )
  {
    _BalancingController.AttachDataLine( _BalanceDataIn[i], _BalanceDataOut[i] );
  }
  
  // Attach Linear Controller
  _lm.Log("--Attaching Linear System Controller.");
  for( unsigned i = 0; i < NUM_LIN_PARAMS; ++i )
  {
    _LinearController.AttachDataLine( _LinearDataIn[i], _LinearDataOut[i] );
  }
  
  // Initialize Devices
  _lm.Log("--Initializing Device Handlers.");
  for( unsigned i = 0; i < NUM_DEVICE_HANDLERS; ++i )
  {
     _DeviceHandlers[i]->Initialize();
  }
  
  _lm.Log("IMCS Ready.");
}

void loop()
{
  
}
    
    

