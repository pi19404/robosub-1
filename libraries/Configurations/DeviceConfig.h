#ifndef __DeviceConfig_H__
#define __DeviceConfig_H__

// (??what kind??) addresses of the accelerometer, compass, and gyro
#define ACCEL_ADDR 0x1D

// these are the I2C addresses of the warious daisy chained pmods
#define I2C_ADDR_PMOD_TEMP      0x4B


#define COMPASS_ADDR 0x19

// depth sensor information
#define DEPTH_PIN A7
#define DEPTH_SCALE (5.0L / 12.0L)

// the serial addresses (??verify??) of each motor
#define M1_ADDR 128
#define M2_ADDR 129
#define M3_ADDR 130

// the number of thrusters 
#define NUM_THRUSTERS 6

// values beyond these can burn out the thrusters
#define PWM_MAX  127
#define PWM_MIN -127

// the number of pneumatic actuators;
// Note: not neccessarily the number of pneumatic devices;
// for example, one of the claws has two actuators (rotate and grab)
#define NUM_PNEUMATICS 7


#endif
