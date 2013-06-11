# Flags passed to the preprocessor.
CPPFLAGS += $(shell echo `pkg-config --cflags opencv`)

# Flags passed to the C++ compiler.
CXXFLAGS += -Wall -Wextra
# The ROBOSUB_CXXFLAGS variable allows the master Makefile to set either -g or
# -O3. To override this, uncomment the line below:
#ROBOSUB_CXXFLAGS := -g
CXXFLAGS += $(shell echo $(ROBOSUB_CXXFLAGS))

LIBS += $(shell echo `pkg-config --libs opencv`)
CXX = g++

OBJS =

all: $(OBJS)

# Print out a variable from the commandline. E.g., "make print-CPPFLAGS"
print-%:
	@echo '$($*)'

objects: $(OBJS)
	@echo "Objects made: $(OBJS)"

clean:
	$(RM) *.o
	$(RM) *.out

