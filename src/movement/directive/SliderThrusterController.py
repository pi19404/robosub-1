# SLIDER GUI DEBUGGER

import pygame, sys
from pygame.locals import *
import math
import time

from Slider import MySlider
from SimpleButton import MySimpleButton

#Copy pasta imports
#import json
#import os
#sys.path.append(os.path.abspath("../.."))
#from util.communication.grapevine import Communicator


#Button Codes
class ButtonCode():
    INVALID = -1
    STOP = 0
    FORWARD = 1
    TURNLEFT = 2
    TURNRIGHT = 3
    BACKWARD = 4
    STRAFELEFT = 5
    STRAFERIGHT = 6
    DIVE = 7
    SURFACE = 8
    LOCK = 9
    TOGGLE = 10
    MODE = 11
    HOVER = 12


class SliderDebugger():
    """
    INFO!
    """
    def __init__(self, screenSize=(600, 600),listen_only=False):
        # set up pygame
        pygame.init()

        # set up the window
        self.windowSurface = pygame.display.set_mode(screenSize, 0, 32)
        if listen_only:
            pygame.display.set_caption('Movement Eavesdropper')
        else:
            pygame.display.set_caption('Thruster Controller')
        self.windowSize = screenSize
        # set up the colors
        self.BLACK = (0, 0, 0)
        self.WHITE = (255, 255, 255)
        self.RED = (255, 0, 0)
        self.GREEN = (0, 255, 0)
        self.BLUE = (0, 0, 255)

        #Submarine Variables
        self.buttonThrustSetting = 0.5
        self.dive_lock = False
        self.publish_interval = 0.100 #seconds
        self.latest_publish_time = 0

        #set mode, depending on what we do
        if listen_only:
            self.mode = "Listener"
        else:
            self.mode = "Controller"
        
        # set up fonts
        print "Attempting to initialize Font (if I fail, this is probably what went wrong!!)"
        try:
            self.basicFont = pygame.font.SysFont("monospace", 15)
        except:
            print "RUH ROH! Font type problem!  I probably crashed :("

        if(self.basicFont):
            print "Font Initialized!"


        #Setup Top Menu Text
        self.statusText = self.basicFont.render(self.getStatusText(), True, self.WHITE, (255, 200, 255))
        self.statusTextRect = self.statusText.get_rect()
        self.statusTextRect.left, self.statusTextRect.top = (0,0)
        self.statusTextRect.width, self.statusTextRect.height = (self.windowSize[0]/2,40)


        #implement sliders (cheez & fried onions)
        self.sliders = []
        self.slider_positions = [[self.windowSize[0]/2,0],
                                 [self.windowSize[0]/2,self.windowSize[1]/3],
                                 [self.windowSize[0]/2,2*self.windowSize[1]/3],
                                 [3*self.windowSize[0]/4,2*self.windowSize[1]/3],
                                 [3*self.windowSize[0]/4,self.windowSize[1]/3],
                                 [3*self.windowSize[0]/4,0]] # in order [ A,B,C,D,E,F]
        self.slider_angles = [135,90,225,135,-90,-135]
        self.slider_size = [self.windowSize[0]/5,self.windowSize[1]/4]
        for i in range(len(self.slider_positions)):
            self.sliders.append(MySlider(self.slider_positions[i], self.windowSurface,
                                         self.slider_size, 0, 0, self.BLUE,
                                         self.slider_angles[i]*math.pi/180.))

        #implement buttons
        self.buttons = []
        self.button_size = [self.windowSize[0]/11,self.windowSize[1]/11]
        self.button_txt = ["TrnL",
                           "FWD",
                           "TrnR",
                           "StfL",
                           "STOP",
                           "StfR",
                           "TOGL",
                           "BWD",
                           "LOCK",
                           "MODE",
                           "HOVR"]
        self.button_codes = [ ButtonCode.TURNLEFT,
                              ButtonCode.FORWARD,
                              ButtonCode.TURNRIGHT,
                              ButtonCode.STRAFELEFT,
                              ButtonCode.STOP,
                              ButtonCode.STRAFERIGHT,
                              ButtonCode.TOGGLE,
                              ButtonCode.BACKWARD,
                              ButtonCode.LOCK,
                              ButtonCode.MODE,
                              ButtonCode.HOVER]
        self.button_pos = [
             [ self.windowSize[0]/10 + self.windowSize[0]/16, self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 2*self.windowSize[0]/10 + self.windowSize[0]/16, self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 3*self.windowSize[0]/10 + self.windowSize[0]/16, self.windowSize[1]/8 + self.windowSize[1]/4],

             [ self.windowSize[0]/10 + self.windowSize[0]/16, 2*self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 2*self.windowSize[0]/10 + self.windowSize[0]/16, 2*self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 3*self.windowSize[0]/10 + self.windowSize[0]/16, 2*self.windowSize[1]/8 + self.windowSize[1]/4],

             [ self.windowSize[0]/10 + self.windowSize[0]/16, 3*self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 2*self.windowSize[0]/10 + self.windowSize[0]/16, 3*self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 3*self.windowSize[0]/10 + self.windowSize[0]/16, 3*self.windowSize[1]/8 + self.windowSize[1]/4],

             [ 2*self.windowSize[0]/10 + self.windowSize[0]/16, 4*self.windowSize[1]/8 + self.windowSize[1]/4],
             [ 3*self.windowSize[0]/10 + self.windowSize[0]/16, 4*self.windowSize[1]/8 + self.windowSize[1]/4]
            ]
        for i in range(len(self.button_txt)):
            self.buttons.append(
                MySimpleButton(self.windowSurface, self.button_pos[i],
                               self.button_size,self.button_txt[i],((70+i*20)%255),self.button_codes[i])
                )


    def drawOther(self):
        # draw the white background onto the surface
        self.windowSurface.fill(self.WHITE)

        self.drawGrid(75,(222,222,222))
        def tmp(x):
            print x
        for s in self.sliders:
            s.draw()
        for b in self.buttons:
            b.draw()

        #Draw Status
        self.statusText = self.basicFont.render(self.getStatusText(), True, (0,0,0), (20,200,20))
        #pygame.draw.rect(self.window, (200,200,200), self.textRect, 0)
        self.windowSurface.blit(self.statusText, self.statusTextRect)

        
    def drawGrid(self, squareSize, color):
        """
        Draws a centered grid of size specified. for debugging purposes
        """
        w,h = self.windowSize
        #draw horizontal lines
        for y in range(0-(h%squareSize), h+squareSize/2, squareSize):
            pygame.draw.line(self.windowSurface, color, (0,y), (w,y), 1)
        for x in range(0-(w%squareSize), w+squareSize/2, squareSize):
            pygame.draw.line(self.windowSurface, color, (x,0), (x,h), 1)

    def setThrusterValues(self, A,B,C,D,E,F):
        thrust_vals = [ A,B,C,D,E,F]
        for i in range(len(thrust_vals)):
            self.sliders[i].setValue(thrust_vals[i])
    
    def buttonAction(self, buttonCode, *largs):
        print "Button Code:", buttonCode
        
        if buttonCode == ButtonCode.STOP:
            self.setThrusterValues( 0,0,0,0,0,0 )
        elif buttonCode == ButtonCode.FORWARD:
            self.setThrusterValues( self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    self.buttonThrustSetting,
                                    self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    self.buttonThrustSetting )
        elif buttonCode == ButtonCode.TURNLEFT:
            self.setThrusterValues( -self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    -self.buttonThrustSetting,
                                    self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    self.buttonThrustSetting )
        elif buttonCode == ButtonCode.TURNRIGHT:
            self.setThrusterValues( self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    self.buttonThrustSetting,
                                    -self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    -self.buttonThrustSetting )
        elif buttonCode == ButtonCode.STRAFELEFT:
            self.setThrusterValues( -self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    self.buttonThrustSetting,
                                    -self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    self.buttonThrustSetting )
        elif buttonCode == ButtonCode.STRAFERIGHT:
            self.setThrusterValues( self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    -self.buttonThrustSetting,
                                    self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    -self.buttonThrustSetting )
        elif buttonCode == ButtonCode.BACKWARD:
            self.setThrusterValues( -self.buttonThrustSetting,
                                    self.sliders[1].value,
                                    -self.buttonThrustSetting,
                                    -self.buttonThrustSetting,
                                    self.sliders[4].value,
                                    -self.buttonThrustSetting )
        elif buttonCode == ButtonCode.HOVER:
            self.setThrusterValues( 0,
                                    self.sliders[1].value,
                                    0,
                                    0,
                                    self.sliders[4].value,
                                    0 )
        elif buttonCode == ButtonCode.TOGGLE:
            self.buttonThrustSetting = (self.buttonThrustSetting+0.1)%1.0  #ranges [0.1,1.0]
            if not self.buttonThrustSetting: self.buttonThrustSetting = 0.1
        elif buttonCode == ButtonCode.LOCK:
            self.dive_lock = not self.dive_lock
            self.sliders[1].lock = self.sliders[4].lock = self.dive_lock
            self.sliders[1].dirtyLockValue = self.dive_lock #to lock them up initially
        elif buttonCode == ButtonCode.MODE:
            #set mode, depending on what we do
            if self.mode == "Controller":
                self.mode = "Listener"
                pygame.display.set_caption('Movement Eavesdropper')
            else:
                self.mode = "Controller"
                pygame.display.set_caption('Thruster Controller')
            #send a safety stop command
            self.setThrusterValues( 0,0,0,0,0,0 )
            
            #TODO: ADD PUBLISH ALL ZEROS
            
            
    def handleLockedSliders(self):
        if self.sliders[1].dirtyLockValue:
            self.sliders[4].setValue(self.sliders[1].value)
        if self.sliders[4].dirtyLockValue:
            self.sliders[1].setValue(self.sliders[4].value)
        self.sliders[1].dirtyLockValue = False
        self.sliders[4].dirtyLockValue = False

    def keyCommands(self, keyevent):
        if keyevent.key == K_UP or keyevent.key == K_w:
            self.buttonAction(ButtonCode.FORWARD)
        elif keyevent.key == K_DOWN or keyevent.key == K_s:
            self.buttonAction(ButtonCode.BACKWARD)
        elif keyevent.key == K_RIGHT or keyevent.key == K_d:
            self.buttonAction(ButtonCode.TURNRIGHT)
        elif keyevent.key == K_LEFT or keyevent.key == K_a:
            self.buttonAction(ButtonCode.TURNLEFT)
        elif keyevent.key == K_e:
            self.buttonAction(ButtonCode.STRAFERIGHT)
        elif keyevent.key == K_q:
            self.buttonAction(ButtonCode.STRAFELEFT)
        elif keyevent.key == K_SPACE:
            self.buttonAction(ButtonCode.STOP)
        elif keyevent.key == K_t:
            self.buttonAction(ButtonCode.TOGGLE)
        elif keyevent.key == K_m:
            self.buttonAction(ButtonCode.MODE)
        elif keyevent.key == K_l:
            self.buttonAction(ButtonCode.LOCK)
        elif keyevent.key == K_RCTRL or keyevent.key == K_LCTRL or keyevent.key == K_x:
            self.buttonAction(ButtonCode.HOVER)
        #Set the Thrust key by percent
        elif keyevent.key == K_1:
            self.buttonThrustSetting = 0.1
        elif keyevent.key == K_2:
            self.buttonThrustSetting = 0.2
        elif keyevent.key == K_3:
            self.buttonThrustSetting = 0.3
        elif keyevent.key == K_4:
            self.buttonThrustSetting = 0.4
        elif keyevent.key == K_5:
            self.buttonThrustSetting = 0.5
        elif keyevent.key == K_6:
            self.buttonThrustSetting = 0.6
        elif keyevent.key == K_7:
            self.buttonThrustSetting = 0.7
        elif keyevent.key == K_8:
            self.buttonThrustSetting = 0.8
        elif keyevent.key == K_9:
            self.buttonThrustSetting = 0.9
        elif keyevent.key == K_0:
            self.buttonThrustSetting = 1.0
                
            
    def getStatusText(self):
        txt = self.mode + " | "
        if self.dive_lock:
            txt += "DIVE LOCKED"
        else:
            txt += "DIVE UNLOCKED"
        txt += " | Thrust: " + str(self.buttonThrustSetting) + " "
        return txt

    def getSliderValues(self):
        s_vals = []
        for s in self.sliders:
            s_vals.append(s.value)
        return s_vals
    
    def listenToMovement(self):
        #This is a silly stub that pushes some things
        self.setThrusterValues( 0.1, 0.2, 0.3, 0.4, 0.5, 0.6 )

    def publishCommands(self):
        next_publish_time = self.latest_publish_time + self.publish_interval
        if next_publish_time <= time.time() and self.mode == "Controller":
            #here is the foo publish command
            print self.getSliderValues()
            self.latest_publish_time = time.time()
        
    def run(self):
        # run the game loop
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type == MOUSEBUTTONUP:
                    for s in self.sliders:
                        s.onClickUp()
                    for b in self.buttons:
                        b.onClickUp(self.buttonAction)
                if event.type == MOUSEBUTTONDOWN:
                    for s in self.sliders:
                        s.onClickDown(event.pos)
                    for b in self.buttons:
                        b.onClickDown(event.pos)
                if event.type == MOUSEMOTION:
                    for s in self.sliders:
                        s.onMouseMove(event.pos)
                if event.type == KEYDOWN:
                    self.keyCommands(event)

            #handle lock
            if self.dive_lock:
                self.handleLockedSliders()

            if self.mode == "Listener":
                self.listenToMovement()
            else:
                self.publishCommands()
            
            self.drawOther()
            pygame.display.update()


if __name__ == "__main__":
    """
    -c : in the command line opens in Controller mode.
         Normally just opens in listener mode.
    """
    started = False
    for arg in sys.argv:
        if arg == "-c": #Start in Command Mode !
            sd = SliderDebugger()
            started = True
    if not started: #Start in Listener Only Mode!
        sd = SliderDebugger(listen_only=True)
    sd.run()

