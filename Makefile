TARGET       = Arduino
ifdef DEBUG
BOARD_TAG    = mega_pic32_dbg
else
BOARD_TAG    = mega_pic32
endif

USER_LIB_PATH = libraries

ARDUINO_LIBS = Wire \
               Wire/utility \
               SPI \
               HardwareSerial \
               ComPort \
               MotorCtrl \
               Gyro \
               Accel \
               Compass 

ARDUINO_PORT = /dev/ttyUSB*

include makefiles/chipKIT.mk

## update system includes -> must follow 'include' line ##
SYS_INCLUDES += $(patsubst %,-I%,$(USER_LIB_PATH))

## define compass gyro and accelerometer -> must follow 'include' line ##
CPPFLAGS += -DCOMP='HMC6343' \
            -DGYR='L3G4200D' \
            -DACL='ADXL345'
