################################################################################
# RoboSub AI Core
# 
################################################################################

import sys
import command.py

## GLOBALS

#actionDict = {'forward': moveForward(), 'stop': stop() } # Etc Etc, Create another file for the dict creation
#decisionDict = {} # Also make another file for this, import in
#obstacleDict = {'gate': gate(), 'path': path(), 'obstacleCourse': obstacleCourse(), 'dropper': dropper(), 'torpedo': torpedo(), 'feedEmperor': feedEmperor(), 'laurelWreath': laurelWreath, 'sonar':sonar }
heading = Pos(0,0,0)
centered = 0
	

## CLASSES 

class missionPlan: 
	buoy    = ["red","green","yellow"]
	torpedo = ["red", "blue"]
	dropper = ["shield", "sword"]

class Pos:
	def __init__(self, x=0, y=0, z=0):
		self.x = x
		self.y = y
		self.z = z

class Obst:
	def __init__(self, name="obst", pos=Pos(), scale=0, rot=0, color=None, task="task", depth=0, heading=None):
		self.name     = name
		self.rot      = rot
		self.pos      = pos
		self.color    = color
		self.task     = task
		self.depth	  = depth
        self.heading  = heading

	def __eq__(self, other):
		return self.name == other.name

## FUNCTIONS

def getInput():
	line = sys.stdin.readline()
	# To handle the '<' and '>' at beginning and end of the input
	line1 = line[1:]
	line2 = line1[:-2]
	n = line2.split(',')
	return n

def analyseInput(item):

	# Parse item info 
	obst = parse(item)

	center = Pos(0,0,0)
    sub = Arduino() # Initialize object for calls to the hardware
	
	if obst.name == "gate":
		sub.move(obst.pos.x,obst.pos.y,obst.pos.z)

	elif obst.name == "path":
		obst.pos = setHeading(obst.heading)
        move(obst.pos)

	elif obst.name == "buoy":
		if obst.color in missionPlan.buoy:
		    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
	elif obst.name == "obstacleCourse":
		sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		

	elif obst.name == "torpedo":
		# Center Obstacle in field of view
		if obst.pos.y > 3: # Turn till it's in the center
		    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
		else:
			stop()
			centerTarget(obst.pos)
			if centered:
				if obst.color == "blue":
                    sub.fire_torpedo_one()
				if obst.color == "red":
                    sub.fire_torpedo_two()

	elif obst.name == "dropper":
		if obst.pos != center:
		    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
		else:
			if obst.image in missionPlan.dropper:
				if obst.image == missionPlan.dropper[0]:
                    sub.drop_marker_one()
				elif ovst.image == missionPlan.dropper[1]:
                    sub.drop_marker_two()

	elif obst.name == "feedEmperor":
		if obst.pos.y > 1: #if not at location:
		    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		
		else:
			stop()
			claw()

		# Add in actual controls for claw and minute accuracies 

	elif obst.name == "laurelWreath":
		print "laurel Wreath"

def parse(message):
	name = message[0]
	name1= name[1:]
	name2 = name1[:-1]
	p = Pos(x=message[2],y=message[3],z=message[4])
	obst = Obst(name=name2,color=message[1], pos=p)

	return obst

def loadObstacle(name):
	# Get relevant information about the obstacle
	# Goal of obstacle
	return 	obstacleDict(name)
    	
#def move(pos):
	#print "<{},{},{}>".format(pos.x, pos.y, pos.z)

def stop():
	print "stop moving"

#def claw():
	#print "activate claw"

def setHeading(pos):
	heading = pos
	move(pos)	

def centerTarget(pos):
	pos.y = 0
    sub.move(obst.pos.x,obst.pos.y,obst.pos.z)		

def shoot(color, pos):
	if color == "red":
		print "Launch red torpedo"
	if color == "blue":
		print "Launch blue torpedo"

def drop(n):
	if n == 0:
		print "not dropping"
	elif n == 1:
		print "Drop marker 1"
	elif n == 2:
		print "Drop marker 2"

def mainLoop():
	listInput = getInput() # Get input from sub programs through message program 
    runProgram() # Ben is writing a program that will be able to start and stop the sub programs when needed.
	analyseInput(listInput) # Analyse the results/output from the program 
	
if __name__ == "__main__":
    input = None
    while not input: # Wait for the start button to be pressed
        input = getInput()
        
    move(0,5,0) # Initial move forward (hopefully through gate)

	while True: # Run until program is fully finished
		mainLoop()

##############################
########## Notes #############
##############################

	# Determine how far away obstacle is
	# Get to specified distance from the obstacle
	#if centered:
		#if not at correct distance: # Move to the correct distance
			#move = howFar(obstacle.scale)

	# Determine actions for obstacle, including special actions
	#if centerd and at distance:
		#Do action	
	
	#if task complete:
		#set task as done

	# Do Actions
	#doAction( 0, turn )
	#doAction( 1, move )
		
#def doAction( index, action ): # Get commands from Jay that he needs 
	#send commands to Jay 
	#if   index == 0: # Turn

	#elif index == 1: # Move

	#elif index == 2: # Launch Torpedo
		
	#elif index == 3: # Release Marker

	#elif index == 4: # Activate Claw

	#elif index == 5: # Activate Sonar
