#Daniel Retherford
#RoboSub Movement Program

##TODO
##
##Maintain depth check, control power to motors, Add ability to incremental power increase/decrease, improve the shut off function
##

#Imported Libraries
from msvcrt import getch
import time


#Functions
#Controls the depth of the robo sub
def userChangeDepth(keyPress):
    x = 0
    speed = 2.0
    speedAdjustment = 2.3
    if keyPress == "down":
        #Move sub down
        for x in range(0, 3):
            print("Sub moving down at", speed)
            speed = speed + speedAdjustment
            time.sleep(1)
    elif keyPress == "up":
        #Move sub up
        for x in range(0, 3):
            print("Sub moving up at", speed)
            speed = speed + speedAdjustment
            time.sleep(1)
    
    speed = 2.0 #set back to original value
    return

#Controls the forward and backwards movement of the sub
def userForwardBackwardChange(keyPress):
    if keyPress == "forward":
        print("Sub is moving forward at")
    elif keyPress == "backwards":
        print("Sub is moving backwards at")
    return

#Controls the rotational movement left and right of the sub
def userRotationChange(keyPress):
    if keyPress == "left":
        print("Sub is rotating to the left at")
    elif keyPress == "right":
        print("Sub is rotating to the right at")

def main():
    while True:
        #Used to capture the keypress
        key = ord(getch())
        #224 is the dafault special key for the keyboard
        if key == 224: #Special keys (arrows, f keys, ins, del, etc.)
            key = ord(getch())

            #Mini Code block bellow controls the up and down
            if key == 81: #Down arrow
                userChangeDepth("down")
            elif key == 73: #Up arrow
                userChangeDepth("up")
            

            #Code block below is code to control direction "left, right, forward, backward"
            #Code is to rotate left and right    
            elif key == 75: # left arrow
                userRotationChange("left")
            elif key == 77: # right arrow
                userRotationChange("right")
                
            #Code is to move forward and backward
            elif key == 72: # up arrow
                userForwardBackwardChange("forward")
            elif key == 80: # down arrow
                userForwardBackwardChange("backwards")

        #Code to shut down all moters
        #Outside of if block because there is no speical key needed
        if key == 27:
            print("sub motors shutting down")
            while True:
                keyBreak = ord(getch())
                if keyBreak == 32:
                    print("sub motors turning on")
                    break
if __name__ == "__main__":
    main()
        
