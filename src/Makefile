# Flags passed to the C++ compiler.
CXXFLAGS += -Wall -Wextra
# The ROBOSUB_CXXFLAGS variable allows the master Makefile to set either -g or
# -O3. To override this, uncomment the line below:
#ROBOSUB_CXXFLAGS := -g
CXXFLAGS += $(shell echo $(ROBOSUB_CXXFLAGS))

CXX = g++

SRC = $(CURDIR)
LIB_DECISION = $(SRC)/decision
LIB_MOVEMENT = $(SRC)/movement
LIB_STATE_MACHINE = $(SRC)/state_machine
LIB_VISION = $(SRC)/vision

MY_TARGET_FOLDERS = $(LIB_DECISION) $(LIB_MOVEMENT) \
                    $(LIB_STATE_MACHINE) $(LIB_VISION)

LIBS += $(shell echo `pkg-config --libs opencv`)
# Add locations to look for .o files.
$(foreach dir,$(MY_TARGET_FOLDERS),$(eval LIBS += -L$(dir));)

# Flags passed to the preprocessor.
CPPFLAGS += $(shell echo `pkg-config --cflags opencv`)
# Add the paths so that we can use #include "file.h" instead of
# #include "../path/to/file.h"
ROBOSUB_INCLUDES =
$(foreach dir,$(MY_TARGET_FOLDERS),$(eval ROBOSUB_INCLUDES += -I$(dir));)
CPPFLAGS += $(ROBOSUB_INCLUDES)

OBJS += main.o

.PHONY: all set_cascade_target_all set_cascade_target_clean $(MY_TARGET_FOLDERS) clean find_robosub_objs

all: robosub.out

find_robosub_objs:
	$(foreach dir,$(MY_TARGET_FOLDERS),$(eval OBJS += $(shell ls $(dir)/*.o)))

robosub.out: set_cascade_target_objects $(MY_TARGET_FOLDERS) \
             find_robosub_objs $(OBJS)
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

set_cascade_target_objects:
	$(eval CASCADE_TARGET := "objects")

objects: $(OBJS)

set_cascade_target_clean:
	$(eval CASCADE_TARGET := "clean")

clean: set_cascade_target_clean $(MY_TARGET_FOLDERS)
	$(RM) *.o
	$(RM) *.out

# Print out a variable from the commandline. E.g., "make print-CPPFLAGS"
print-%:
	@echo '$($*)'

$(MY_TARGET_FOLDERS):
	@$(MAKE) -C $@ $(CASCADE_TARGET) ROBOSUB_INCLUDES="$(ROBOSUB_INCLUDES)";

