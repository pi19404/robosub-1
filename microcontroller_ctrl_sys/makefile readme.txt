to build:
make

to upload to arduino:
make upload (this sets a flag in the arduino make file)

to build and upload in one command line:
make && make upload

Global definitions in Makefile (the make file in 'john_arduino' directory):
    Under 'CPPFLAGS += ', use '-D' followed by the #define (no spaces) you want 
    to define.  
    For example, if I want to define the global LOGGING, I write the 
    following: "-DLOGGING='1'.  From the first '-' to the ending apostrophe of 
    the definition, there must not be any spaces.

    To add a new defition, terminate the last line of the CPPFLAGS assignment
    with a backslash ('\'), add a new line, then add the definition as 
    described above.
    For example, if the last definition is "-DLOGGING='1'" and I want to add
    a definition for a flag that allows me to run the RoboSubController source
    file that tests the thrusters, I would add a '\' to the end of the line, 
    then add a new line in which I write 
    "-DROBOSUBCONTROLLER_TESTING_THRUSTERS"

    These are the acceptable flags:
    HAVE_COMPASS
        Enables the declaration of compass code in the IMU.  Must be 
        followed by a COMPASS_MODEL_... flag to enable the definitions for the 
        compass in use.

    COMPASS_MODEL='chip model in single quotes'
        Current value: 'HMC6343'
        Defines the model number (not the make) of the compass being used and
        enables the source code for that model.
        As of this writing, the HMC6343 is the only compass we have, but there 
        is still a flag for it in case we eventually get one with a different 
        chip.

    HAVE_GYRO
        Enables the declaration of gyroscope code in the IMU.  Must be 
        followed by an GYRO_MODEL_... flag to enable the definitions for the 
        gyroscope in use.

    GYRO_MODEL='chip model in single quotes'
        Current value: 'L3G4200D'
        Defines the model number (not the make) of the gyroscope being used and
        enables the source code for that model.
        As of this writing, the L3G4200D is the only gyroscope we have, but 
        there is still a flag for it in case we eventually get one with a 
        different chip.

    HAVE_ACCEL
        Enables the declaration of accelerometer code in the IMU.  Must be 
        followed by an ACCEL_MODEL_... flag to enable the definitions for the 
        accelerometer in use.

    ACCEL_MODEL='chip model in single quotes'
        Current value: 'ADXL345'
        Defines the model number (not the make) of the accelerometer being 
        used and enables the source code for that model.
        As of this writing, the ADXL345 is the only accelerometer we have, but 
        there is still a flag for it in case we eventually get one with a 
        different chip.

    CLS_IS_I2C
        Enables the source code for the CLS pmod that allows it to opperate on
        the I2C line.  Do not define this flag if the CLS is not in use or not
        on the I2C line.

    LOGGING
        Turns the logging function on and off.  If on, any logging action will
        send a string through Serial1.  It is expected that a PC will be 
        reading the string through MPIDE's Serial Monitor.
        Logging functions on: '1'
        Logging functions off: '0' or not defined at all

    ROBOSUBCONTROLLER_RELEASE
        Enables the RoboSubController source code that runs the release version
        of the controller's code.
        Not compatible with any other flag that begins with "ROBOSUBCONTROLLER"
        Does not require a value.  It only needs to be defined, as if there was
        a "#define THING" statment without a value.
        Used in RoboSubController_release.cpp.

    ROBOSUBCONTROLLER_TESTING_THRUSTERS
        Enables the RoboSubController source code that runs some thruster
        testing routines.
        Not compatible with any other flag that begins with "ROBOSUBCONTROLLER"
        Does not require a value.  It only needs to be defined, as if there was
        a "#define THING" statment without a value.
        Used in RoboSubController_testing_thrusters.cpp.

    ROBOSUBCONTROLLER_TESTING_DEPTH_RUN
        Enables the RoboSubController source code that runs the depth demo that
        was performed on Friday morning, 3-29-2013 at the UI pool.  This code
        will read commands from the PC, sanitize the thruster values, and send
        those values to the thrusters.  The controller will also periodically
        (exact period time is specified in milliseconds in the code) read from
        the depth sensor and send that data to the PC.
        Not compatible with any other flag that begins with "ROBOSUBCONTROLLER"
        Does not require a value.  It only needs to be defined, as if there was
        a "#define THING" statment without a value.
        Used in RoboSubController_release.cpp.

    ROBOSUBCONTROLLER_TESTING_PRESSURE
        Enables the RoboSubController source code that reads from the pressure
        sensor.

    ROBOSUBCONTROLLER_TESTING_ACCEL
        Enables the RoboSubController source code that reads from the 
        accelerometer.  Must have an accelerometer class defined (see below).

    ROBOSUBCONTROLLER_TESTING_GYRO
        Enables the RoboSubController source code that reads from the 
        gyroscope.  Must have a gyroscope class defined (see below).

    ROBOSUBCONTROLLER_TESTING_CLS
        Enables the RoboSubController source code that writes some strings to
        the CLS pmod.  Must have the CLS class defined (see below).

    ROBOSUBCONTROLLER_TESTING_COMPASS
        Enables the RoboSubController source code that reads from the 
        compass.  Must have a compass class defined (see below).

    ROBOSUBCONTROLLER_TESTING_CLS_GYRO
        Enables the RoboSubController source code that reads from the gyroscope
        and writes the data to the CLS pmod.  Must have gyroscope and CLS 
        classes defined (see below).

    ROBOSUBCONTROLLER_TESTING_PNEUMATICS
        Enables the RoboSubController source code that activates and 
        deactivates the pneumatic solenoids.

    ROBOSUBCONTROLLER_TESTING_JOYSTICK_RUN
        Enables the RoboSubController source code that monitors the serial port
        for commands from the PC and performs the appropriate action.
        This code was built for a demonstration test in David's pool.
        The sub's PC receives commands over a network cable from a PC with a 
        joystick.  The joystick is not important to this microcontroller code
        though.



