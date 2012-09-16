#ifndef __MOTORCTRL_CFG_DATA_H__
#define __MOTORCTRL_CFG_DATA_H__

// Motor MotorDirection command
// is Motor::Enum + MotorCommand::Enum
namespace Motor {
enum Enum {
     One = 0
    ,Two = 4
};
}
namespace MotorDirection {
enum Enum {
     Forward  = 0
    ,Backward = 1
};
}

// LowResolution commands
namespace LowResMotor {
enum Enum {
     One = 6
    ,Two = 7
};
}

// Voltage commands
namespace MotorVoltageCommand {
enum Enum {
     MinVoltage = 2
    ,MaxVoltage = 3
};
}

#define MOTOR_MAX_SPEED 127
#define SCALE_MAX_SPEED(s) ( ((s) > MOTOR_MAX_SPEED) ? MOTOR_MAX_SPEED : (s) )
#define MIN_VOLT_MAX 30.0
#define MAX_VOLT_MAX 25.0
#define SCALE_MIN_VOLT_VAL(v) ( ((v) > MIN_VOLT_MAX) ? MIN_VOLT_MAX : (v) )
#define SCALE_MAX_VOLT_VAL(v) ( ((v) > MAX_VOLT_MAX) ? MAX_VOLT_MAX : (v) )
#define MIN_VOLT_TO_VAL(v) ( (SCALE_MIN_VOLT_VAL(v)-6.0)*5.0 )
#define MAX_VOLT_TO_VAL(v) (  SCALE_MAX_VOLT_VAL(v)*5.12 )

#endif //__MOTORCTRL_CFG_DATA_H__
