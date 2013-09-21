CC=g++
AR=ar
ARFLAGS=rvs
I_DIR=../../include
L_DIR=../../lib
CFLAGS=-c -Wall -Werror $(DEBUG) -I$(I_DIR)
LDFLAGS=-L$(L_DIR) -static
TARGET=libav8r.a
HF=av8r.h
LIB=-lrealjoy
SRC=av8r.cpp
OBJ=$(SRC:.cpp=.o)

LIB_REALJOY_DIR=../librealjoy
LIB_REALJOY=$(LIB_REALJOY_DIR)/librealjoy.a

all: $(SRC) $(TARGET) $(LIB_REALJOY)
 
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

.PHONY: $(LIB_REALJOY)
$(LIB_REALJOY):
	cd $(LIB_REALJOY_DIR); $(MAKE) && $(MAKE) install

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $(TARGET) $(OBJ)

.cpp.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ $(LIB)
