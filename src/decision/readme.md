Decision & Artificial Intelligence
==================================
Our agents have a heiarchy of decision makers, each having a hand in the command 
message handed to our movement modules, which are assumed to complete our task 
fully.

Heirarchy
---------
> ### Master AI (AI State Machine) >> "ai_state_machine.py" ###
> * Monitors failure states of slave AI
> * Debugging Selection Menu for User
> * Allows switching between lower AI modules.
> * Handles some start up cases
> * *TODO?:* Startup (or command other module to startup) needed deamons and modules 

> > ###  Task AI Modules ###
> > * Actually gives commands to movement
> > * Usually each module performs one particular task
> > * Communicates with Master AI, usually success, or failure

Task AI Modules
--------------
####Live Task AI Modules####
Task AI that is complete and approved.  Typically integrated into our main Branch.
- Not Applicable. . . .

####Completed Task AI Modules####
All Task AI that is in a currently finished state, but not in main branch.
- Base AI   >> "base_oligarch.py"
- Keyboard Controller 
- Depth Holding >> "depth_oligarch.py"

####In-Progress Task AI Modules####
AI that is currently being developed.
- Depth holding w/ Keyboard Controls
- Path Following

####Future Task AI Modules####
Tasks that will be created.
- Torpedo Shooter
- Pylon Rodeo
- Unnassisted Travel Agent (for traveling when no landmarks are visible)

