CC=g++
B_DIR=../../bin
I_DIR=../../include
L_DIR=../../lib
CFLAGS=-c -Wall -Werror $(DEBUG) -I$(I_DIR)
LDFLAGS=-L$(L_DIR) -static
TARGET=joytest
LIB=-lav8r -lrealjoy
SRC=joytest.cpp
OBJ=$(SRC:.cpp=.o)
LIB_OBJ=libav8r.a

all: $(LIB_OBJ) $(SRC) $(TARGET) 

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-ggdb"

.PHONY: install
install:
	mkdir -p $(B_DIR)
	cp -u $(TARGET) $(B_DIR)

.PHONY: clean
clean:
	rm -rf ./*.o

.PHONY: $(LIB_OBJ:.a=)
%.a: %
	cd ../$< ; $(MAKE) && $(MAKE) install

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $< $(LIB)

.cpp.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LIB)
