#ifndef COMPASS_H
#define COMPASS_H

// Note: Local for this file only!
#define LOG_STRING_LEN 128

// This data structure is built with influence from Table 2 - HMC6343 Interface
// Commands/Responses on page 10 of the HMC6343 manual.  Each read command 
// causes the compass chip to send 6 bytes of data over the I2C line, each byte
// only interrupted by waiting for an acknowledge bit from the master (this
// microcontroller).  We are using command byte 0x50, which reads 2 bytes of
// heading data (MSB followed by LSB), 2 bytes of pitch data (same byte order)
// from the accelerometer, and 2 bytes of roll data (same byte order) from the
// accelerometer.  We could use another command to read all three axes of the
// magnetometer, but then we just read raw data.  By reading the heading 
// through command byte 0x50, the heading (according to page 7 of the HMC63643
// manual, "forward" when mounted level is the X axis) magnetometer data is run
// through onboard filters.  We want this.

// this data structure must be visible outside this file, so it is declared 
// outside the HAVE_COMPASS flag
typedef struct compassReading
{
    float heading;
    float pitch;
    float roll;
} COMPASS_DATA;

#ifdef HAVE_COMPASS

class compass
{
public:
    compass();

    // reads all three axes on the ADXL345 gyro chip and fills the provided
    // argument structure with the data
    void readAllAxes(COMPASS_DATA *putDataHere);

private:
    // for log strings that need to be constructed with variable data;
    char mLogString[LOG_STRING_LEN];
};

#else
// the makefile says that no compass is attached, so make a dummy class so that
// the IMU is happy

class compass
{
public:
    compass() {}

    void readAllAxes(COMPASS_DATA *putDataHere)
    {
        putDataHere->heading = 0.0;
        putDataHere->pitch = 0.0;
        putDataHere->roll = 0.0;
    }
};

#endif  // HAVE_COMPASS

#endif  // COMPASS_H

