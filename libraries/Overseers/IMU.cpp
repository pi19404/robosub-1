#include <Overseers/IMU.h>

IMU::IMU()
{
    // sensor class members (accel, compass, and gyro) have their default 
    // constructors (format = classname(*no argument*)), which I defined, 
    // called by the compiler when they are declared as member objects (as 
    // opposed to pointers or references), and the depth sensor just requires
    // an analog reading, so no initialization necessary there
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("IMU::IMU - IMU initialized");
}

void IMU::getAccelAll(ACCEL_DATA *putHere)
{
    // read and return all accelerometer handler data
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("IMU::getAccelAll(...) - start reading accel data");
    mAccel.readAllAxes(putHere);
    mLogInstance.LogStr("IMU::getAccelAll(...) - done accel data");
}

void IMU::getCompassAll(COMPASS_DATA *putHere)
{
    // read and return all compass handler data
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("IMU::getCompassAll(...) - start reading compass data");
    mCompass.readAllAxes(putHere);
    mLogInstance.LogStr("IMU::getCompassAll(...) - done reading compass data");
}

void IMU::getGyroAll(GYRO_DATA *putHere)
{
    // read and retrieve all gyro handler data
    LogManager& mLogInstance = LogManager::GetInstance();

    mLogInstance.LogStr("IMU::getGyroAll(...) - start reading gyro data");
    mGyro.readAllAxes(putHere);
    mLogInstance.LogStr("IMU::getGyroAll(...) - done reading gyro data");
}

uint16_t IMU::readDepth(void)
{
    // Explanation of depth sensor readings and data manipulation
    /*
        The depth sensor is actually a pressure sensor designed for underwater
        operation.  It's model number is US381-000005-030PA.  The last section
        is the one that interests us.  
        According to the data sheet for the sensor, the '030' means that the
        operating range is 30 (0 - 30), the 'P' means that the units are in
        psi, and the 'A' means that the reading is absolute (that is, 
        atmospheric pressure is not accounted for in the readings).
        The data sheet also says the sensor puts out 4mA - 20mA.  Although not
        expressly stated, we believe that this corresponds to 4mA for its 
        lowest reading (0psi) and 20mA for its highest reading (30psi).

        The shield circuit contains an Instrument Amplifier circuit.  The 
        pressure sensor is plugged into this circuit.  This circuit will take
        the sensor's small current range and create a voltage signal that is
        within the voltage range of the chipKIT's input pins.  The shield is 
        designed (in 2012 by the RoboSub EE team) to route a voltage signal for
        this pressure sensor to one particular pin on the chipKIT.  When run
        through this instrument Amplifier circuit, the pressure sensor's read
        range of 4mA - 20mA will be converted into a voltage range of 0.64V - 
        3.20V.  The chipKIT will perform an analog read of this pin.

        The analog read will run the analog signal through an analog-digital
        converter (ADC).  The ADC has an output range of 0-1023.  Additionally,
        the ADC circuit, being part of the chipKIT, is powered by chipKIT's
        peripheral circuitry power, which is 3.3V.  It's input range is equal to this (??verify??)
        Thus, a one-digit change in the ADC output means a change of 
        3.3/1023 ~= 0.00323V.
        If the ADC reads 3.3V from the shield pin that is connected to the 
        pressure sensor, then that means that something is wrong.  The pressure
        sensor cannot put out more than 20mA, which corresponds to 3.20V on the
        chipKIT pin, so if 3.3V are put out, something is very wrong.

        We now know that a one-digit change in the ADC output corresponds to a
        0.00323V change on the Instrument Amplifier output.  The pressure 
        sensor operating range is 0 - 30psi.  At 30psi, the output will be 
        20mA, or 3.2V at the Instrument Amplifier output.  This corresponds to 
        a 3.2V / 0.00323V ~= 991 value on the digital pin.  This relationship
        in turn allows us to figure out the psi change per digit according to
        the following equation: 30psi / 991 = 0.0303 psi/digit.

        Brief but related subject change.

        We know that, at sea level (which we are assuming here), barometric
        pressure is 1 atm.  This is equivalent to 14.7 psi.
        We also know that, as you go down in the water, 10m (or 34.1475ft) of
        depth will result in a 1 atm pressure increase.
        Thus, we can find the psi per feet in depth by the following equation:
        34.1475ft / 14.7psi = 2.3230 ft/psi.

        Therefore, a one-digit change on the digital pin that is connected to
        this pressure sensor corresponds to 
        2.3230 ft/psi * 0.0303 psi/digit = 0.0704 ft/digit.

        We can break it down further into inches by multiplying this value by
        12.

        To get the depth below the water, we perform an analog read from the
        pin and perform the following calculations:
        - subtract ((14.7 psi) / (0.0303 psi/digit)) to subtract 1 atmospheric 
        pressure (the pressure at sea level)
        - multiply the remainder by 0.0704 ft/digit to get the depth of the sub
        below the water in feet
        
    */

//    float myF = 0.0;
//    int16_t depthDigital = 0;
    int16_t depthAnalog = 0;

    int psi;
    int depthInFeet;

    LogManager& mLogInstance = LogManager::GetInstance();

    // read directly from the pin connecting to the depth sensor;
    mLogInstance.LogStrHex("IMU::readDepth - start reading = ", depthAnalog);
    depthAnalog = analogRead(DEPTH_PIN);
    mLogInstance.LogStrHex("IMU::readDepth - depth data = ", depthAnalog);

    // calculate the psi reading from the analog reading
    psi = depthAnalog * 0.0303;

    // subtract atmospheric pressure at sea level
    psi -= 14.7;

    // calculate depth
    depthInFeet = psi * 2.323;
    mDepthDataInches = depthInFeet * 12;

#ifdef BLUEBIRD
    myF = 14.7/0.0303;
    mLogInstance.LogStrFloat("IMU::readDepth - 14.7/0.0303 = ", myF);
    myF = (float)depthAnalog - myF;
    mLogInstance.LogStrFloat("IMU::readDepth - depthdigital - that = ", myF);
    myF = myF * 0.0704;
    mLogInstance.LogStrFloat("IMU::readDepth - that * 0.0704 = ", myF);
    myF = myF * 12.0;
    mLogInstance.LogStrFloat("IMU::readDepth - that * 12.0 = ", myF);

//    mDepthDataIn = (((float)depthDigital - (14.7 / 0.0303)) * 0.0704) * 12;
    mDepthDataIn = myF;
    mLogInstance.LogStrHex("IMU::readDepth - depth = ", mDepthDataIn);

/*
    LogManager& mLogInstance = LogManager::GetInstance();
    snprintf(mLogString, LOG_STRING_LEN, 
        "IMU::readDepth - Read depth '%d'in from depth sensor.", mDepthDataIn);
    mLogInstance.Log(mLogString);
*/
//    mDepthDataIn = analogRead(DEPTH_PIN);
//    snprintf(mLogString, LOG_STRING_LEN, 
//        "IMU::readDepth - Read depth '%x'in from depth sensor.", mDepthDataIn);
//    mLogInstance.LogStr(mLogString);
#endif

    // return the depth data
    return mDepthDataInches;
}

