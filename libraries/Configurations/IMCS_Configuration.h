#ifndef __IMCS_CONFIGURATION_H__
#define __IMCS_CONFIGURATION_H__

// LIST (ARRAY) GENERATION MACROS
#include <MacroTricks/MacroTricks.h>

// BEGIN DEVICE CREATION
#include <Math/Vector.h>
#define DEVICES      _Devices
#define DEVICES_TYPE DeviceHandler*
#define NUM_DEVICES  LIST_N_ELEMS(DEVICES,DEVICES_TYPE)
#define DEVICES_LIST LIST(DEVICES_TYPE,DEVICES,DEVICE)

// MOTOR CONTROLLERS:
#define M1_ADDR 128
#define M2_ADDR 129
#define M3_ADDR 130

#define MOTOR_CTRLS      _MotorCtrls
#define MOTOR_CTRLS_TYPE RealMotorCtrlHandler*
#define NUM_MOTOR_CTRLS  LIST_N_ELEMS(MOTOR_CTRLS,MOTOR_CTRLS_TYPE)
#define MOTOR_CTRLS_LIST LIST(MOTOR_CTRLS_TYPE,MOTOR_CTRLS,MCTRL)

#include <HardwareSerial.h>
#include <ComPort/SerialComPort.h>
#include <MotorCtrl/RealMotorCtrlHandler.h>

SerialComPort _M1Com( Serial1, M1_ADDR );
SerialComPort _M2Com( Serial1, M2_ADDR );
SerialComPort _M3Com( Serial1, M3_ADDR );

RealMotorCtrlHandler _M1( _M1Com );
RealMotorCtrlHandler _M2( _M2Com );
RealMotorCtrlHandler _M3( _M3Com );

#define MCTRL1 LIST_FIRST_ELEM(&_M1, MCTRL2)
#define MCTRL2 LIST_ELEM(&_M2, MCTRL3 )
#define MCTRL3 LIST_ELEM(&_M3, MCTRL4 )
#define MCTRL4 LIST_END

MOTOR_CTRLS_LIST;

#define DEVICE1   LIST_FIRST_ELEM( MOTOR_CTRLS[0], DEVICE1_1 )
#define DEVICE1_1 LIST_ELEM( MOTOR_CTRLS[1], DEVICE2 )

// THRUSTERS
#include <Thruster/RealThrusterPlant.h>
#include <Thruster/RealThruster.h>

#define THRUSTERS      _Thrusters
#define THRUSTERS_TYPE RealThruster*
#define NUM_THRUSTERS  LIST_N_ELEMS(THRUSTERS,THRUSTERS_TYPE)
#define THRUSTERS_LIST LIST(THRUSTERS_TYPE,THRUSTERS,THRUSTER)

RealThrusterPlant THRUSTER_PLANT;

RealThruster _T1( *MOTOR_CTRLS[2], Motor::One );  // strafing 1
RealThruster _T2( *MOTOR_CTRLS[2], Motor::Two );  // strafing 2
RealThruster _T3( *MOTOR_CTRLS[0], Motor::One );  // forward 1
RealThruster _T4( *MOTOR_CTRLS[1], Motor::One );  // forward 2
RealThruster _T5( *MOTOR_CTRLS[0], Motor::Two );  // vertical 1
RealThruster _T6( *MOTOR_CTRLS[1], Motor::Two );  // vertical 2

#define THRUSTER1 LIST_FIRST_ELEM( &_T1, THRUSTER2 )
#define THRUSTER2 LIST_ELEM( &_T2, THRUSTER3 )
#define THRUSTER3 LIST_ELEM( &_T3, THRUSTER4 )
#define THRUSTER4 LIST_ELEM( &_T4, THRUSTER5 )
#define THRUSTER5 LIST_ELEM( &_T5, THRUSTER6 )
#define THRUSTER6 LIST_ELEM( &_T6, THRUSTER7 )
#define THRUSTER7 LIST_END

THRUSTERS_LIST;
Vector<ThrusterData,NUM_THRUSTERS> THRUSTERS_DATA;

// PNEUMATICS:
#include <PneumaticController/RealPneumaticPlant.h>
#include <PneumaticController/RealPneumaticController.h>
#include <PneumaticController/RealTimedPneumaticController.h>

#define PNEUMATICS      _Pneumatics
#define PNEUMATICS_TYPE RealPneumaticController*
#define NUM_PNEUMATICS  LIST_N_ELEMS(PNEUMATICS,PNEUMATICS_TYPE)
#define PNEUMATICS_LIST LIST(PNEUMATICS_TYPE,PNEUMATICS,PNEUMATIC)

RealPneumaticPlant PNEUMATIC_PLANT;

RealTimedPneumaticController _Torpedo1 ( 52,  500 );
RealTimedPneumaticController _Torpedo2 ( 53,  500 );
RealTimedPneumaticController _Marker1  ( 49, 1500 );
RealTimedPneumaticController _Marker2  ( 43, 1500 );
RealPneumaticController      _Claw1Grip( 41       );
RealPneumaticController      _Claw1Rot ( 31       );
RealPneumaticController      _Claw2Grip( 41       );

#define PNEUMATIC1 LIST_FIRST_ELEM(&_Torpedo1, PNEUMATIC2)
#define PNEUMATIC2 LIST_ELEM(&_Torpedo2, PNEUMATIC3)
#define PNEUMATIC3 LIST_ELEM(&_Marker1,  PNEUMATIC4)
#define PNEUMATIC4 LIST_ELEM(&_Marker2,  PNEUMATIC5)
#define PNEUMATIC5 LIST_ELEM(&_Claw1Grip,PNEUMATIC6)
#define PNEUMATIC6 LIST_ELEM(&_Claw1Rot, PNEUMATIC7)
#define PNEUMATIC7 LIST_ELEM(&_Claw2Grip,PNEUMATIC8)
#define PNEUMATIC8 LIST_END

PNEUMATICS_LIST;
Vector<PneumaticControllerData, NUM_PNEUMATICS> PNEUMATICS_DATA;

#define DEVICE2   LIST_ELEM( PNEUMATICS[0], DEVICE2_1 )
#define DEVICE2_1 LIST_ELEM( PNEUMATICS[1], DEVICE2_2 )
#define DEVICE2_2 LIST_ELEM( PNEUMATICS[2], DEVICE2_3 )
#define DEVICE2_3 LIST_ELEM( PNEUMATICS[3], DEVICE2_4 )
#define DEVICE2_4 LIST_ELEM( PNEUMATICS[4], DEVICE2_5 )
#define DEVICE2_5 LIST_ELEM( PNEUMATICS[5], DEVICE2_6 )
#define DEVICE2_6 LIST_ELEM( PNEUMATICS[6], DEVICE5   )

// Compass
#define COMPASS_ADDR 0x19
#include <Wire.h>
#include <ComPort/WireComPort.h>
#include <Compass/CompassData.h>
#include <Compass/RealCompassHandler.h>

WireComPort _CompassCom( Wire, COMPASS_ADDR );
RealCompassHandler COMPASS( _CompassCom );

CompassData<float> COMPASS_DATA;

#define DEVICE5 LIST_ELEM(&COMPASS, DEVICE6)

// Depth Sensor
#include <DepthSensor/RealDepthSensor.h>
#define DEPTH_PIN A7
int DEPTH_DATA;
#define DEPTH_SCALE (5.0L / 12.0L)
#define DEVICE6 LIST_GO_TO( DEVICE7 )

// Begin Interrupt
#include <Device/Interrupt.h>
#define RUN_PIN 2
#define RUN_INT 0
//Interrupt RUN_INTERRUPT( RUN_INT, RUN_PIN );
bool _RUN = true;

#define DEVICE7 LIST_GO_TO( DEVICE8 )//LIST_ELEM( &RUN_INTERRUPT, DEVICE8 )

#define DEVICE8 LIST_END

DEVICES_LIST;

#endif //__IMCS_CONFIGURATION_H__
