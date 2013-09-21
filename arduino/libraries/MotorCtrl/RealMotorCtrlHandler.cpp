#include "RealMotorCtrlHandler.h"
#include <ComPort/ComPort.h>

void RealMotorCtrlHandler::Initialize()
{
    // Command Motors to Stop
    DriveMotor( Motor::One, MotorDirection::Forward, 0 );
    DriveMotor( Motor::Two, MotorDirection::Forward, 0 );
}

void RealMotorCtrlHandler::DriveMotor( Motor::Enum motor, int16_t velocity )
{
    MotorDirection::Enum dir;
    if( velocity < 0 )
    {
        velocity = -velocity;
        dir = MotorDirection::Backward;
    }
    else
    {
        dir = MotorDirection::Forward;
    }
    
    DriveMotor( motor, dir, (uint8_t)SCALE_MAX_SPEED(velocity) );
}
    
void RealMotorCtrlHandler::DriveMotor( Motor::Enum motor, MotorDirection::Enum dir, uint8_t speed )
{
    uint8_t command = (uint8_t)motor + (uint8_t)dir;
    SendCommand(command, (uint8_t)SCALE_MAX_SPEED(speed));    
}

void RealMotorCtrlHandler::DriveMotorLowRes( LowResMotor::Enum motor, uint8_t speedDir )
{
    uint8_t command = (uint8_t)motor;
    SendCommand(command, (uint8_t)SCALE_MAX_SPEED(speedDir));
}

void RealMotorCtrlHandler::SetMinVoltage( float voltage )
{
    uint8_t command = (uint8_t)(MotorVoltageCommand::MinVoltage);
    uint8_t val = (uint8_t)MIN_VOLT_TO_VAL(voltage);
    SendCommand(command,val);
}
         
void RealMotorCtrlHandler::SetMaxVoltage( float voltage )
{
    uint8_t command = (uint8_t)(MotorVoltageCommand::MaxVoltage);
    uint8_t val = (uint8_t)MAX_VOLT_TO_VAL(voltage);
    SendCommand(command,val);
}

void RealMotorCtrlHandler::SendCommand(uint8_t command, uint8_t val )
{
    uint8_t deviceAddress = mComPort.getDevice();
    uint16_t checksum = (deviceAddress + command + val)&127;
    uint8_t data[] = { deviceAddress, command, val, (uint8_t)checksum };
    mComPort.write( data, 4 );
}
