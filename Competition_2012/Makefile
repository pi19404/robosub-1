TARGET       = Competition_2012
ifdef DEBUG
BOARD_TAG    = mega_pic32_dbg
else
BOARD_TAG    = mega_pic32
endif

USER_LIB_PATH = $(dir $(CURDIR))/libraries

ARDUINO_LIBS = Wire \
               Wire/utility \
               SPI \
               HardwareSerial \
               ComPort \
               MotorCtrl \
               Compass 

ARDUINO_PORT = /dev/ttyUSB*

include $(dir $(CURDIR))/makefiles/chipKIT.mk

## update system includes -> must follow 'include' line ##
SYS_INCLUDES += $(patsubst %,-I%,$(USER_LIB_PATH))

## define compass gyro and accelerometer -> must follow 'include' line ##
CPPFLAGS += -DCOMP='HMC6343' \
            -DGYRO='L3G4200D' \
            -DACCEL='ADXL345'
