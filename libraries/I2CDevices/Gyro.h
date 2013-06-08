#ifndef GYRO_H
#define GYRO_H

// Note: Local for this file only!
#define LOG_STRING_LEN 128

// this data structure must be visible outside this file, so it is declared 
// outside the HAVE_GYRO flag
typedef struct gyroReading
{
    float X;
    float Y;
    float Z;
} GYRO_DATA;

#ifdef HAVE_GYRO

class gyro
{
public:
    gyro();

    // reads all three axes on the ADXL345 gyro chip and fills the provided
    // argument structure with the data
    void readAllAxes(GYRO_DATA *putDataHere);

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
// the makefile says that no gyroscope is present, so make a dummy class to 
// keep the IMU happy

class gyro
{
public:
    gyro() {}

    void readAllAxes(GYRO_DATA *putDataHere)
    {
        putDataHere->X = 0.0;
        putDataHere->Y = 0.0;
        putDataHere->Z = 0.0;
    }
};

#endif  // HAVE_GYRO

#endif  // GYRO_H

