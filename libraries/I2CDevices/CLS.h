#ifndef CLS_H
#define CLS_H

// Note: Local for this file only!
#define LOG_STRING_LEN 128

#ifdef CLS_IS_I2C

class CLS
{
public:
    CLS();

    // clears the screen, then writes the supplied string to it
    void writeStrToCLS(char *buffer);

private:
    // for log strings that need to be constructed with variable data;
    char mLogString[LOG_STRING_LEN];

    // blindly writes the specified number of bytes to the CLS;
    void writeNBytes(char *buffer, unsigned int numToWrite);
};

#else
// the makefile does not specify a CLS pmod, so make a dummy class to keep the
// IMU happy

class CLS
{
public:
    CLS() {}

    void writeStrToCLS(char *buffer) {}
};

#endif  // CLS_IS_I2C

#endif  // CLS_H

