####################################################
# To add a target, add the project folder (in src)
# name to the following list. Then, write a Makefile
# for it (see src for examples) that has build 
# targets: "all" and "install", and place that 
# Makefile in your project folder.
####################################################
TARGETS=joytest libav8r librealjoy robosub_joystick control_system

.PHONY: all
all: 
	@echo "Usage:\n make TARGET_NAME"


clean: $(TARGETS:%=%_clean)
	rm -rf bin/ lib/

%_clean:
	cd src/$(@:%_clean=%); $(MAKE) clean
    
$(TARGETS):
	cd src/$@; $(MAKE) && $(MAKE) install
