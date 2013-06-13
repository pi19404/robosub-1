# Flags passed to the preprocessor.
CPPFLAGS += $(shell echo `pkg-config --cflags opencv`)

# Flags passed to the C++ compiler.
CXXFLAGS += -Wall -Wextra
# The ROBOSUB_CXXFLAGS variable allows the master Makefile to set either -g or
# -O3. To override this, uncomment the line below:
#ROBOSUB_CXXFLAGS := -g
CXXFLAGS += $(shell echo $(ROBOSUB_CXXFLAGS))

ROBOSUB_INCLUDES =
CPPFLAGS += $(ROBOSUB_INCLUDES)

LIBS += $(shell echo `pkg-config --libs opencv`)
CXX = g++

OBJS = tollbooth_dlm.o base_dlm.o

all: $(OBJS)

# Print out a variable from the commandline. E.g., "make print-CPPFLAGS"
print-%:
	@echo '$($*)'

objects: $(OBJS)
	@echo "Objects made: $(OBJS)"

clean:
	@echo THAT
	@echo $(ROBOSUB_INCLUDES)
	$(RM) *.o
	$(RM) *.out

