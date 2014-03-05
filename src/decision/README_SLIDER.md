Slider Controller/Listener README
=============
###March 3, 2014###
----------------------------------------------------
####KEY COMMANDS
* SPACE - Stop All (CURRENTLY DOESN'T STOP IN LISTENER MODE)
* w - Forward
* a - Turn Left
* s - Backward
* d - Turn Right
* q - Strafe Left 
* e - Strafe Right


* m - Change Mode
* 0-9 - Change Thrust Command
* t - Toggle Thrust Command by +0.1
* x - Hover
* CTRL - Hover (also)

#### Toggling Thurst Pecentage
Changing what percentage any preset command deals to the movement controls.
* 0 thru 9 keys - Apply command percent (7 is 70% for example)

#### Changing Mode ####
Controller Mode - sends packets to the movement stack.

Listener Mode - listens to thruster values and displays them.
* m - toggles between Controller and Listener mode

NOTE: normally starts in Listener Mode. 
* "-c" - command line areguement opens in Controller Mode

#### Hover Command
Simply turns off thrusters 1,3,4,6 (A,C,D,F), as in, all thrusters besides depth thrusters.

This is for letting depth be maintained while stopping movement on the lateral plane.




