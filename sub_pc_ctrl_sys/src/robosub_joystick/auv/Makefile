CC=g++
R_DIR=../../..
I_DIR=$(R_DIR)/include
L_DIR=$(R_DIR)/lib
B_DIR=$(R_DIR)/bin
CFLAGS=-c -O0 -Wall $(DEBUG) -I$(I_DIR) -I./
LDFLAGS=-static -L$(L_DIR) 
SOURCES=main.cpp robosub_control_data.cpp arduino_data.cpp robosub_controller.cpp
LIBS=-pthread -lboost_regex -lboost_system -lboost_program_options 
EXECUTABLE=auv

OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

.PHONY: clean
clean: 
	rm -rf *.o
	rm -rf $(EXECUTABLE)

.PHONY: install
install: 
	mkdir -p $(B_DIR)
	cp -u ./$(EXECUTABLE) $(B_DIR)/$(EXECUTABLE)

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-ggdb"

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)  

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
