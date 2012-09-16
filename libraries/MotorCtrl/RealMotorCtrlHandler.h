#ifndef __REAL_MOTOR_CTRL_HANDLER_H__
#define __REAL_MOTOR_CTRL_HANDLER_H__

#include "MotorCtrlHandler.h"

class SerialComPort;

class RealMotorCtrlHandler
    :
    public MotorCtrlHandler
{       
    public:
        RealMotorCtrlHandler( ComPort& comPort )
            :
            mComPort(comPort)
        { }
        
        virtual ~RealMotorCtrlHandler() { }
            
        // From DeviceHandler
        virtual void Initialize();
        
        // For Motor Controllers to implement:
        
        /* Drive Motor
         * -description: drives the selected motor in the specified
         * -args: Motor::Enum motor
         * -args: int8_t motor velocity : -128 to 127
         * -returns: none
         */
         virtual void DriveMotor( Motor::Enum, int16_t );
        
        /* Drive Motor
         * -description: drives the selected motor in the specified
         * -description: direction at speed 0 to 127
         * -args: Motor::Enum motor number
         * -args: MotorDirection::Enum motor direction
         * -args: uint8_t motor speed
         * -returns: none
         */
        virtual void DriveMotor( Motor::Enum, MotorDirection::Enum, uint8_t );
        
        /* Drive Motor
         * -description: drives the selected motor at half resolution
         * -description: 0 to 63 reverse, 64 off, 65-127 forward
         * -args: Motor::Enum motor number
         * -args: uint8_t motor direction and speed
         * -returns: none
         */
        virtual void DriveMotorLowRes( LowResMotor::Enum, uint8_t );
        
        /* Min Voltage
         * -description: set the minimum voltage for the battery
         * -description: that powers the motor controller.
         * -args: float voltage (6V to 30V)
         * -returns: none
         */
         virtual void SetMinVoltage( float );
         
         /* Max Voltage
         * -description: set the maximum voltage for driving the motor
         * -args: float voltage (0V to 25V)
         * -returns: none
         */
         virtual void SetMaxVoltage( float );
         
         
    private:
        void SendCommand( uint8_t, uint8_t );
         
    private:
        ComPort& mComPort;
};

#include "RealMotorCtrlHandler_impl.h"

#endif //__REAL_MOTOR_CTRL_HANDLER_H__
