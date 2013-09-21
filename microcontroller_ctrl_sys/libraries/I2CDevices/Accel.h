#ifndef ACCEL_H
#define ACCEL_H

// Note: Local for this file only!
#define LOG_STRING_LEN 128

// this data structure must be visible outside this file, so it is declared 
// outside the HAVE_ACCEL flag
typedef struct accelReading
{
    float X;
    float Y;
    float Z;
} ACCEL_DATA;

#ifdef HAVE_ACCEL

class accel
{
public:
    accel();

    // reads all three axes on the accelerometer chip and fills the provided
    // argument structure with the data
    void readAllAxes(ACCEL_DATA *putDataHere);

private:
    // for log strings that need to be constructed with variable data;
    // it is convenient to have a known string available for the class
    char mLogString[LOG_STRING_LEN];

    // these register reads and writes should not be available outside this 
    // class, so they are declared private
    void readDeviceRegister
        (uint8_t devAddr, uint8_t regAddr, uint8_t *dataByte);

    void writeDeviceRegister
        (uint8_t devAddr, uint8_t regAddr, uint8_t dataByte);
};

#else
// the makefile says that no accelerometer is present, so make a dummy class to
// make the IMU happy

class accel
{
public:
    accel() {}

    void readAllAxes(ACCEL_DATA *putDataHere)
    {
        putDataHere->X = 0.0;
        putDataHere->Y = 0.0;
        putDataHere->Z = 0.0;
    }
};

#endif  // HAVE_ACCEL

#endif  // ACCEL_H

