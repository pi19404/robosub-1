###################################################
# \file joystick_test.sh
###################################################
# Author: 	   Jay Sidhu <jaypal.sidhu@gmail.com>
# Project: 	   RoboSub Joystick Communication
# Description: Initializes the environment,
#              and runs the joystick programs.
#
###################################################
#!/bin/bash

###########################
# SCRIPT DEFINITIONS HERE
###########################
CLIENT_IP=192.168.1.120
AUV_IP=192.168.1.120
AUV_USER=j
JOYSTICK_DEV=/dev/input/js0
ARDUINO_DEV=/dev/ttyACM0
ARDUINO_BAUD=115200
NETCAT_PORT=2000

##########################
# DON'T TOUCH THESE
##########################
INSTALL_DIR=~/robosub_joystick
RUN_DIR=stage
CLIENT_EXEC=client
AUV_EXEC=auv
CLIENT_INSTALL_DIR=$INSTALL_DIR/$CLIENT_EXEC
AUV_INSTALL_DIR=$INSTALL_DIR/$AUV_EXEC
CLIENT_DIR=~/$RUN_DIR/$CLIENT_EXEC
AUV_DIR=$RUN_DIR/$AUV_EXEC

###########################
# FUNCTION DEFINITIONS
##########################
function setup_auv() {

    # Create the run directory on the auv
    ssh $AUV_USER@$AUV_IP "mkdir -p $AUV_DIR"

    # Make the pipe
    ssh $AUV_USER@$AUV_IP "rm -rf $AUV_DIR/pipe; mkfifo $AUV_DIR/pipe"
    
    # Copy auv program to the auv's run folder
    scp $AUV_INSTALL_DIR/$AUV_EXEC $AUV_USER@$AUV_IP:$AUV_DIR
}

function setup_client() {
    # create run directory on the client
    mkdir -p $CLIENT_DIR

    # Make the pipe
    rm -rf $CLIENT_DIR/pipe
    mkfifo $CLIENT_DIR/pipe

    # copy the client program to the run folder 
    cp -f $CLIENT_INSTALL_DIR/$CLIENT_EXEC $CLIENT_DIR
}

function run_client() {
    # spawn the netcat process
    # which will set up a listening session on the client
    ( cat $CLIENT_DIR/pipe | nc -l $CLIENT_IP $NETCAT_PORT ) &
	cat_pid=$!

    # spawn the client terminal
    # which will run the client portion of the 
    # joystick program
    client_str="$CLIENT_DIR/$CLIENT_EXEC -j $JOYSTICK_DEV -o $CLIENT_DIR/pipe"
    ( gnome-terminal --title="CLIENT" -x /bin/bash -c "$client_str" )&
	gnome_term_pid=$!

	CLIENT_PIDS="$cat_pid $gnome_term_pid"
}

function run_auv() {
    # spawn the netcat process 
    # which will complete the netcat session with the client
	auv_args="nc $CLIENT_IP $NETCAT_PORT > $AUV_DIR/pipe"
	auv_str="ssh $AUV_USER@$AUV_IP \"$auv_args\""
    ( gnome-terminal --title="NETCAT -- AUV" -x /bin/bash -c "$auv_str"  ) &
	auv_pid_0=$!

    # spawn the auv terminal
    auv_args="$AUV_DIR/$AUV_EXEC -i $AUV_DIR/pipe -s $ARDUINO_DEV -b $ARDUINO_DEV"
    auv_str="ssh $AUV_USER@$AUV_IP \"$auv_args\""
    ( gnome-terminal --title="AUV" -x /bin/bash -c "$auv_str" ) &
	auv_pid_1=$!

    # spawn the arduino terminal
    ard_args="cat $ARDUINO_DEV"
    ard_str="ssh $AUV_USER@$AUV_IP \"$ard_args\""
    ( gnome-terminal --title="ARDUINO" -x /bin/bash -c "$ard_str" ) &
	auv_pid_2=$!

    AUV_PIDS="$auv_pid_0 $auv_pid_1 $auv_pid_2"
}

##################
# SCRIPT BEGIN
##################

echo "Configuring the AUV..."
setup_auv

echo "Configuring the Host..."
setup_client

echo "Running the AUV program..."
run_auv

echo "Running the Host program..."
run_client

echo "Press any key to end the joystick test..."
read val
kill $CLIENT_PIDS
kill $AUV_PIDS
killall -q ssh
killall -q nc
killall -q cat

