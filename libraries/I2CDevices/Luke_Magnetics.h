#ifndef MAGNETICS_H
#define MAGNETICS_H

// Note: Local for this file only!
#define LOG_STRING_LEN 128

// this data structure must be visible outside this file, so it is declared 
// outside the HAVE_COMPASS flag
typedef struct magneticReading
{
    float x;
    float y;
    float z;
} MAGNETIC_DATA;

#ifdef HAVE_MAGNETICS

class magnetics
{
public:
    magnetics();

    // reads all three axes on the MAG3110 magnetic chip and fills the provided
    // argument structure with the data
    void readAllAxes(MAGNETICS_DATA *putDataHere);
    void selectSensor(uint8_t sensorId);
    void magnetics::initMagnetics();

private:
    // for log strings that need to be constructed with variable data;
    char mLogString[LOG_STRING_LEN];
};

#else
// the makefile says that no compass is attached, so make a dummy class so that
// the IMU is happy

class magnetics
{
public:
    magnetics() {}

    void readAllAxes(COMPASS_DATA *putDataHere)
    {
        putDataHere->x = 0.0;
        putDataHere->y = 0.0;
        putDataHere->z = 0.0;
    }
};

#endif  // HAVE_MAGNETICS

#endif  // MAGNETICS_H

