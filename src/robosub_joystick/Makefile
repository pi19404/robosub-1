B_DIR=../../bin
INSTALL_DIR=~/robosub_joystick
AUV=auv
CLIENT=client
RUN_SCRIPT=joystick.sh

all: $(AUV)/$(AUV) $(CLIENT)/$(CLIENT)

.PHONY: install
install:
	cd $(AUV); $(MAKE) install
	cd $(CLIENT); $(MAKE) install
	rm -rf $(INSTALL_DIR)
	mkdir -p $(INSTALL_DIR)/$(AUV)
	mkdir -p $(INSTALL_DIR)/$(CLIENT)
	cp -u $(B_DIR)/$(AUV) $(INSTALL_DIR)/$(AUV)
	cp -u $(B_DIR)/$(CLIENT) $(INSTALL_DIR)/$(CLIENT)
	cp -u $(RUN_SCRIPT) $(INSTALL_DIR)

.PHONY: uninstall
uninstall:
	rm -rf $(INSTALL_DIR)

.PHONY: clean
clean:
	cd $(AUV); $(MAKE) clean
	cd $(CLIENT); $(MAKE) clean

.PHONY: $(AUV)/$(AUV)
$(AUV)/$(AUV):
	cd $(AUV); $(MAKE)

.PHONY: $(CLIENT)/$(CLIENT)
$(CLIENT)/$(CLIENT):
	cd $(CLIENT); $(MAKE)
