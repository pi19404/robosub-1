###############################################################################
# RoboSub AI Core
# 
################################################################################

import comm
import sys
import time


## CLASSES 

class Pos:
    def __init__(self, x=0, y=0, z=0):
        self.strafe  = x
        self.forward = y
        self.depth   = z

class Obst: 
    def __init__(self, name="obst", pos=Pos(), width=0, height=0, rot=0, color=None, depth=1, heading=None, currDepth=0, confirm=None):
        #The known obstacle specs
        self.name     = name
        self.depth	  = depth 

        # Current obst readings
        self.rot      = rot
        self.pos      = pos
        self.color    = color
        self.heading  = heading
        self.currDepth= currDepth
        self.confirm  = confirm

    def __eq__(self, other):
        return self.name == other.name
# We just need to get closer to it, not farther away! -Ben Smith


## GLOBALS
obstacles = []
heading = Pos(0,0,0)
centered = 0
emergency = False 
fov = 1.09      # Field of view in radians
midpoint = 640  # Change if image width isn't 1280
buoyNum = 0     # Primary buoy in list
obstNum = 0     # Current obstacle 
strafeCount = 0 # Counter to determine how far strafed before turning around
surface = False # Boolean to tell when to surface
currObstComplete = False# If False call program for next obstacle

# Mission Plan
buoys    = ["red","green"]
torpedos = ["red", "blue"]
droppers = ["shield", "sword"]

#Tasks Complete?
gateComplete        = False
buoyComplete        = False
obstComplete        = False
torpedoComplete     = False
dropperComplete     = False
feedEmperorComplete = False
laurelComplete      = False
surfaceComplete     = False


## FUNCTIONS

def generateObstacles():
    #Gate
    obstacles.append(Obst(name="gate", depth=.8))  # Depth should be 1 for competition

    #Path
    obstacles.append(Obst(name="path", width=0.15, height=1.2)) 

    #Buoys
    obstacles.append(Obst(name="buoy", width=.23, height=.23, depth=2.5)) 

    #Obstacle Course
    obstacles.append(Obst(name="obstacle", width=.051, height=.6, depth=2.5)) 

    #Torpedos
    obstacles.append(Obst(name="torpedo", width=.17, height=.35, depth=3.5)) 
        # The width value = the smaller circle diameter
        # The height value = the larger circle diameter

    #Dropper
    obstacles.append(Obst(name="dropper", width=.15, height=.6, depth=3.5)) 

    #Feed Emperor
    obstacles.append(Obst(name="feedEmperor", width=.15, height=.6, depth=2.5)) 

    #Laurel Wreath
    obstacles.append(Obst(name="laurel", width=.15, height=.6, depth=3.5)) 

def getInput():
    line = sys.stdin.readline()
    # To handle the '<' and '>' at beginning and end of the input
    line1 = line[1:]
    line2 = line1[:-2]
    n = line2.split(',')
    return n

def analyseInput(item, sub):

    # Parse item info 
    obst = parse(item)
   
    center = Pos(0,0,0)
    #sub = Arduino() # Initialize object for calls to the hardware
    
    # Gate 
    if obst.name == "gate":
        if gateComplete:
            sub.y = .3
            obstNum += 1
            return
        if confirm == '{':
            foundGate = True
        if confirm == '?' and foundGate:
            if obst.pos.x > 0:
                obst.pos.x = .3
            elif obst.pos.x < 0:
                obst.pos.x = -.3
            if obst.pos.z > 0:
                obst.pos.z = .3
            elif obst.pos.z < 0:
                obst.pos.z = -.3
            sub.move(obst.pos.x,.3,obst.pos.z)
        elif confirm == '?':
            sub.heading_offset(10) # Rotate until full gate found
        #print "gate"

    # Path
    elif obst.name == "path":
        sub.heading_offset(obst.rot)
        sub.y = .3
        #print "Path"


    elif obst.name == "buoy":
        if buoyComplete:
            sub.move(0,.3,.3) # Move past obstacle to orange path
            obstNum += 1
            return

        if obst.color == buoys[buoyNum]: 
            if obst.percent > 70: # buoy is very close
                if obst.pos.x > 0:
                    obst.pos.x = .3
                elif obst.pos.x < 0:
                    obst.pos.x = -.3
                if obst.pos.z > 0:
                    obst.pos.z = .3
                elif obst.pos.z < 0:
                    obst.pos.z = -.3
                sub.move(obst.pos.x,.3,obst.pos.z) # continue moving towards buoy
                #determine if needed
                time.sleep(2) # Wait for 3 seconds to verify buoy has been hit
                if buoyNum == 0:
                    buoyNum = 1 # Switch buoy we're looking for to secondary
                else:
                    buoyNum = None
                    buoyComplete = True # Set buoy task as complete

                sub.move(0,-.3,0) # Back up until second buoy has been found
            else:
                if obst.pos.x > 0:
                    obst.pos.x = .3
                elif obst.pos.x < 0:
                    obst.pos.x = -.3
                if obst.pos.y > 0:
                    obst.pos.y = .3
                elif obst.pos.y < 0:
                    obst.pos.y = -.3
                if obst.pos.z > 0:
                    obst.pos.z = .3
                elif obst.pos.z < 0:
                    obst.pos.z = -.3
                sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
            #print "{} bouy".format(obst.color)
        else:
            if obst.percent > 40:
                sub.move(0,-.3,0) # Move back to see others
            elif strafeCount < 10:
                sub.move(.3,0,0) # Strafe right
                strafeCount+=1
            else:
                sub.move(-.3,0,0) # Strafe left 

    elif obst.name == "obstacleCourse":
        if obstComplete:
            sub.move(0,.3,.3) # Move past obstacle to orange path
            obstNum += 1
            return

        if obst.pos.x > 0:
            obst.pos.x = .3
        elif obst.pos.x < 0:
            obst.pos.x = -.3
        if obst.pos.y > 0:
            obst.pos.y = .3
        elif obst.pos.y < 0:
            obst.pos.y = -.3
        if obst.pos.z > 0:
            obst.pos.z = .3
        elif obst.pos.z < 0:
            obst.pos.z = -.3
        sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
        #print "obstacle course"

    elif obst.name == "torpedo":
        if redTorpedoComplete and blueTorpedoComplete:
            sub.move(0,.3,.3) # Move past obstacle to orange path
            obstNum += 1
            return
            
        # Center Obstacle in field of view
        if obst.pos.y > 3: # Turn till it's in the center
            if obst.pos.x > 0:
                obst.pos.x = .3
            elif obst.pos.x < 0:
                obst.pos.x = -.3
            if obst.pos.y > 0:
                obst.pos.y = .3
            elif obst.pos.y < 0:
                obst.pos.y = -.3
            if obst.pos.z > 0:
                obst.pos.z = .3
            elif obst.pos.z < 0:
                obst.pos.z = -.3
            sub.move(obst.pos.x,obst.pos.y,obst.pos.z)	
        else:
            stop()
            centerTarget(sub,obst.pos)
            #if centered:
            if obst.color == torpedos[0]:
                sub.fire_torpedo_one()
            if obst.color == torpedos[1]:
                sub.fire_torpedo_two()
            

    elif obst.name == "dropper":
        if dropper1Complete and dropper2Complete:
            sub.move(0,.3,.3) # Move past obstacle to orange path
            obstNum += 1
            return

        if obst.pos.x != center.x and obst.pos.z != center.z:
            if obst.pos.x > 0:
                obst.pos.x = .3
            elif obst.pos.x < 0:
                obst.pos.x = -.3
            if obst.pos.y > 0:
                obst.pos.y = .3
            elif obst.pos.y < 0:
                obst.pos.y = -.3
            sub.y = obst.pos.x # Move forwared or backward
            sub.x = obst.pos.y # Move side to side
        else:
            if obst.image in droppers:
                if obst.image == droppers[0]:
                    sub.drop_marker_one()
                elif ovst.image == droppers[1]:
                    sub.drop_marker_two()

    elif obst.name == "feedEmperor":
        if obst.pos.y > 1: #if not at location:
            if obst.pos.x > 0:
                obst.pos.x = .3
            elif obst.pos.x < 0:
                obst.pos.x = -.3
            if obst.pos.y > 0:
                obst.pos.y = .3
            elif obst.pos.y < 0:
                obst.pos.y = -.3
            if obst.pos.z > 0:
                obst.pos.z = .3
            elif obst.pos.z < 0:
                obst.pos.z = -.3
		    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
        else:
            stop()
            claw()

    elif obst.name == "laurelWreath":
        print "laurel Wreath"

def parse(message):
    name = message[0]
    name1= name[1:]
    name2 = name1[:-1]
    
    obst = loadObstacle(name2)
    obst.color=message[1]
    obst.pos = Pos(x=message[2],y=message[3],z=message[4])
    obst.rot = message[5]

    return obst

def loadObstacle(name):
    # Get relevant information about the obstacle
    # Goal of obstacle
    i=0
    while i < len(obstacles):
        if name == obstacles[i].name:
            return obstacles[i]
        i+=1
    	
def stop(sub):
    sub.move(0,0,0)
    #print "stop moving"

def strafe(direction):
    # Direction = positive or negative x value
    sub.move(direction) # strafe in direction 

def angleX(fov, midpoint, x):
    if x >= midpoint:
        return (fov/2) * x/(midpoint * 2)
    else:
        return ((fov/2) * x/midpoint) * -1

def centerTarget(sub,pos):
    while pos != center:
        #maintainDistance()
        rot = angleX(fov,midpoint,pos.x) # determine angle to turn
        sub.heading_offset(rot)	

def checkDepth(sub): # Modify to only change z, waiting on interface
    if surface:
        sub.y = .3
    elif sub.depth < .5: # Don't let it surface
        sub.y = -.3
        #print "Submerge"
    elif sub.depth > 3: # Don't go too low
        sub.y = .3
        #print "rise"
    elif sub.depth < (obstacles[obstNum].depth - .2):
        sub.y = .3
    elif sub.depth > (obstacles[obstNum].depth + .2):
        sub.y = -.3
    else:
        sub.y = 0

def waitForArmed(sub): # Ready to start, waiting on start button
    try:
        sub.wait_for_armed(True)
    finally:
        sub.stop()

def runProg(prog):
    pass #waiting on Ben
    
def mainLoop(sub):
    checkDepth(sub) # Verifies correct depth
    
    # Notes of how to start programs
    #list [program, arguments,...]
    #color x y z confirm

    #if currObstComplete:
    #   runProg(obstacles[obstNum].name)

    ####################################33
    # Commented out for depth check
    #listInput = getInput() # Get input from sub programs through message program 
    #analyseInput(listInput, sub) # Analyse the results/output from the program 


	
if __name__ == "__main__":
    generateObstacles() # builds the list of obstacles
    sub = comm.Arduino("/dev/ttyACM0") # Initialize object for calls to the hardware
    sub.go() # Starts active communication with the arduino

    waitForArmed(sub) # Wait for the start button to be pressed
        
    sub.move(0,0,-.3) # Initial move down and forward in meters, until depth reached

    while True: # Run until program is fully finished
        mainLoop(sub)
