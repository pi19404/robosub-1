CC=g++
AR=ar
ARFLAGS=rvs
I_DIR=../../include
L_DIR=../../lib
CFLAGS=-c -Wall -Werror $(DEBUG) -I$(I_DIR)
TARGET=librealjoy.a
HF=real_joystick_handler.h
SRC=real_joystick_handler.cpp
OBJ=$(SRC:.cpp=.o)

all: $(SRC) $(TARGET)

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-ggdb"

.PHONY: clean
clean:
	rm -rf ./*.o
	rm -rf ./*.a

.PHONY: install
install:
	mkdir -p $(L_DIR)
	cp -u $(HF) $(I_DIR)
	cp -u $(TARGET) $(L_DIR)

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
