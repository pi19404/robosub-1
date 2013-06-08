TARGET       = Arduino

# see *mpide dir*/hardware/pic32/boards.txt
#BOARD_TAG = cerebot_mx4ck
BOARD_TAG = mega_pic32

USER_LIB_PATH = libraries

ARDUINO_LIBS =  Wire \
                Wire/utility \
                SPI \
                HardwareSerial \
                ComPort \
                I2CDevices \
                Pneumatics \
                Thruster \
                Overseers \
                Logging \
                RoboSubController

ARDUINO_PORT = /dev/ttyUSB*

# MPIDE_DIR
# This variable must be set to point
# to your mpide installation. Do not set it here.
# Add the following line (uncommented) to your .profile and .bashrc:
# export MPIDE_DIR=/path/to/your/mpide/installation

# Include the chipkit makefile definitions
include makefiles/chipKIT.mk

## update system includes -> must follow 'include' line ##
SYS_INCLUDES += $(patsubst %,-I%,$(USER_LIB_PATH))

## define compass gyro and accelerometer -> must follow 'include' line ##
## also put the logging flag here, which says whether or not the log manager
## is active ##
CPPFLAGS +=             \
-DLOGGING='1'           \
-DROBOSUBCONTROLLER_TESTING_THRUSTERS


