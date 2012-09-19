# We just need to get closer to it, not farther away! -Ben Smith


class Obst: 
    def __init__(self, name="obst", pos=Pos(), width=0, height=0, rot=0, color=None, depth=2, heading=None, currDepth=0):
        #The known obstacle specs
        self.name     = name
        self.depth    = depth 
      
        # Current obst readings
        self.rot      = rot
        self.pos      = pos
        self.color    = color
        self.heading  = heading
        self.currDepth= currDepth

    def __eq__(self, other):
        return self.name == other.name
    
obstacles = []

if "__name__" == "__main__":
    #Gate
    obstacles.append(Obst(name="gate", depth=1)) 

    #Path
    obstacles.append(Obst(name="path", width=0.15, height=1.2, depth=1)) 

    #Buoys
    obstacles.append(Obst(name="buoy", width=.23, height=.23, depth=1)) 

    #Obstacle Course
    obstacles.append(Obst(name="obstacle", width=.051, height=.6, depth=1)) 

    #Torpedos
    obstacles.append(Obst(name="torpedo", width=.17, height=.35, depth=1)) 
        # The width value = the smaller circle diameter
        # The height value = the larger circle diameter

    #Dropper
    obstacles.append(Obst(name="dropper", width=.15, height=.6, depth=1)) 

    #


    


