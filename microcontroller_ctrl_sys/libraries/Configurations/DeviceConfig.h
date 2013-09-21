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

// the number of thrusters 
#define NUM_THRUSTERS 6

// the serial addresses (??verify??) of each motor
#define M1_ADDR 128
#define M2_ADDR 129
#define M3_ADDR 130

// the number of pneumatic solenoids;
#define NUM_PNEUMATICS 6

// the pin numbers that control each solenoid
/*
    A pneumatic device has two pneumatic hoses connected to it, and the way 
    that those hoses are connected to the solenoids determines the solenoid 
    that powers each device, and therefore also the arduino's pin number 
    corresponding to that device.

    Details can be found in the drop box in the document titled
    "Shield Pinout.xlsx".

    Current (7-6-2013) connection:
    Device      Location        Solenoid #
    --------------------------------------
    Torpedo 1   port            3
    Torpedo 2   starboard       6
    Marker 1    port            1
    Marker 2    starboard       4
    Claw 1      port            2
    Claw 2      starboard       5

*/
#define solenoid1Pin 52
#define solenoid2Pin 53
#define solenoid3Pin 49
#define solenoid4Pin 43
#define solenoid5Pin 41
#define solenoid6Pin 31



#endif
