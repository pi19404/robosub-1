################################################################################
# RoboSub AI Core
# 
################################################################################

import sys



## GLOBALS

#actionDict = {'forward': moveForward(), 'stop': stop() } # Etc Etc, Create another file for the dict creation
#decisionDict = {} # Also make another file for this, import in
#obstacleDict = {'gate': gate(), 'path': path(), 'obstacleCourse': obstacleCourse(), 'dropper': dropper(), 'torpedo': torpedo(), 'feedEmperor': feedEmperor(), 'laurelWreath': laurelWreath, 'sonar':sonar }

missionPlan = ["gate", "path", "buoy", "obstacle", "dropper", "torpedo"]


## CLASSES 

class Pos:
	def __init__(self, x=0, y=0, z=0):
		self.x = x
		self.y = y
		self.z = z

class Obst:
	def __init__(self, name="obst", pos=Pos(), scale=0, rot=0, color=None, task="task", depth=0):
		self.name     = name
		self.rot      = rot
		self.pos      = pos
		self.color    = color
		self.task     = task
		self.depth	  = depth

	def __eq__(self, other):
		return self.name == other.name

#class Task:
	#navigate
	#torpedo
	#marker
	#claw
	#hydophone


## FUNCTIONS

def getInput():
	line = sys.stdin.readline()
	# Need to handle the '<' and '>' at beginning and end of the input
	n = line.split(',')
	return n

def analyseInput():

	# Read inputs
	#for item in listInput:
		# Define var as an obstacle

		obstacle = Obst(name="buoy")

		line = sys.stdin.readline()
		# Need to handle the '<' and '>' at beginning and end of the input
		line1 = line[1:]
		line2 = line1[:-2]
		message = line2.split(',')

		# Parse item info 
		#obst = parse(n)
		#print "parse"
		name = message[0]
		name1= name[1:]
		name2 = name1[:-1]
		p = Pos(x=message[2],y=message[3],z=message[4])
		obst = Obst(name=name2,color=message[1], pos=p)

		# Load obstacle
	    #obst = loadObstacle(line)
		#obstacle.name = "buoy"	
		#obst.name = "buoy"
        
		# If it's an obstacle we want to do:
			# Determine where obstacle is
		if obst.name == "buoy":
			move(obst.pos)

		

			#if obst.name == "torpedo":


			# Center Obstacle in field of view
			#if not in center: # Turn till it's in the center
				#turn = direction(obstacle.pos, obstacle.rot)

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

def parse(message):
	o = Obst()

	o.name  = message[0]
	o.color = message[1]
	o.pos.x = message[2] 
	o.pos.y = message[3]
	o.pos.z = message[4]

	return o

def loadObstacle(name):
	# Get relevant information about the obstacle
	# Goal of obstacle
	return 	obstacleDict(name)
    	
def move(pos):
	print "<{},{},{}>".format(pos.x, pos.y, pos.z)

#def direction(pos, rot):
	# Add in math here to determine where to turn
	#return turn


#def howFar(scale):
	# Add math here to determine how far to move
	#return move


def mainLoop():
	#listInput = getInput()
	analyseInput()#listInput)
	
if __name__ == "__main__":
	while True:
		mainLoop()
