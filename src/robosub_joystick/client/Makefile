CC=g++
R_DIR=../../..
S_DIR=$(R_DIR)/src
I_DIR=$(R_DIR)/include
L_DIR=$(R_DIR)/lib
B_DIR=$(R_DIR)/bin
CFLAGS=-c -O3 -Wall -I$(I_DIR) -I./
LDFLAGS=-static -L$(L_DIR) 
SOURCES=main.cpp robosub_joystick_publisher.cpp
LIBS=-pthread -lboost_regex -lboost_system -lboost_program_options -lav8r -lrealjoy 
EXECUTABLE=client
LIB_OBJS=libav8r.a

OBJECTS=$(SOURCES:.cpp=.o)

all: $(LIB_OBJS) $(EXECUTABLE)

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

.PHONY: $(LIB_OBJS:.a=)
%.a: % 
	cd $(S_DIR)/$<; $(MAKE) && $(MAKE) install

$(EXECUTABLE): $(OBJECTS)  
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)  

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@
