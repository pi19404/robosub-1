# Robosub #

Robosub Club of The Palouse consists of students from the University of Idaho
and from Washington State University. This club is focused on building an
Autonomous Unmanned Vehicle for the 2014 AUVSI competition in San Diego, CA.
You can find out more about the AUVSI competition at robosub.org.

This project includes wiki documentation that describes the preferred workflow,
coding standards, and compilation process.


## Getting started with keyboard input ##

The program "Dr. Frankenstein's Machine" is intended to be a general purpose
way to launch all of the robosub modules. However, there are currently two
modules that will need to be launched manually.

### Dr. Frankenstein's Machine ###

First, use "Dr. Frankenstein's Machine" to launch the majority of the modules.

    $ cd robosub/src
    $ ./Dr.\ Frankenstein\'s\ Machine -c release

This should "electrify" four modules: movement, decision, util/logging, and
movement/directive. You can make sure that all of these modules are still
running by executing

    $ ./Dr.\ Frankenstein\'s\ Machine -r

### Starting the microcontroller ###

The microcontroller can be started in one of two states. The debug state does
not interact with the microcontroller. The normal state requires superuser
privilage since the usuer typically doesn't have sufficient permissions to
write to to /dev/ttyUSB0.

To start the microtroller interface in debug mode, do:

    $ cd robosub/src/microcontroller
    $ python microcontroller\_interface.py -d

Do start it in "release" mode, do:

    $ cd robosub/src/microcontroller
    # python microcontroller\_interface.py

Keep in mind that the motors generate too much heat to be operated out of
water for any length of time.

### Keyboard control ###

Finally, we need to start the keyboard advisor. This is a program that
alters the AI state machine. If the advisor does not issue any messages,
the AI state machine should (eventually) execute its normal tasks. If the
advisor does issue a command, however, it's able to force a specific state.
One of those states will replace any artificial thoughts with malicious
whisperings.

    $ cd robosub/src/decision/advisor
    $ python Rasputin.py

The numbers represent states. Press "e" to stop the motors (hopefully).

### Debugging ###

One of the modules launched by "Dr. Frankenstein's Machine" was the logger.
This program is writing all grapevine messages to /tmp/robosub/log.out and
flushing the output after every line. Since each message is tagged with
the module name of the source, we can use the "tail" program to watch
the messages coming out of a specific module. To do this, execute

    $ tail -f /tmp/robosub/log.out | grep "module name"

For example, to watch Rasputin.py's messages, execute:

    $ tail -f /tmp/robosub/log.out | grep decision/advisor

