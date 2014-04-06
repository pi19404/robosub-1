# master ai gui


import sys
import os
import time
import math
import json

import pygame
from pygame.locals import *

sys.path.append(os.path.abspath("../tasks"))
from task_code import TaskCode

sys.path.append(os.path.abspath("../../movement/directive"))
from Slider import MySlider
from SimpleButton import MySimpleButton

sys.path.append(os.path.abspath("../.."))
from util.communication.grapevine import * #Communicator


class SliderDebugger():
    """
    INFO!
    """
    def __init__(self, screenSize=(800, 600)):
        self.current_task_code = TaskCode.STANDBY

        # set up pygame
        pygame.init()

        # set up the window
        self.windowSurface = pygame.display.set_mode(screenSize, 0, 32)
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
        
        # set up fonts
        print "Attempting to initialize Font (if I fail, this is probably what went wrong!!)"
        try:
            self.basicFont = pygame.font.SysFont("monospace", 15)
        except:
            print "RUH ROH! Font type problem!  I probably crashed :("

        if(self.basicFont):
            print "Font Initialized!"	
            
        #Initiallize Comms!
        self.com = Communicator(module_name="decision/master_ai")#module_name=args.module_name)

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
        self.button_txt = ["Standby",
                           "FooSquare",
                           "StartGate",
                           "Line Fol",
                           "Hold Depth",
                           "Dive to 2m"
                            ]
        self.button_codes = [ TaskCode.STANDBY,
                              TaskCode.FOO_SQUARE,
                              TaskCode.START_GATE,
                              TaskCode.LINE_FOLLOW,
                              TaskCode.STANDBY_AT_DEPTH,
                              TaskCode.DIVE
                              ]
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
        self.windowSurface.fill((255, 225, 225))

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
        
    def buttonAction(self, button_code, *largs):
        print "Button Code:", button_code
        self.publishMessage(button_code)
            
    def handleLockedSliders(self):
        if self.sliders[1].dirtyLockValue:
            self.sliders[4].setValue(self.sliders[1].value)
        if self.sliders[4].dirtyLockValue:
            self.sliders[1].setValue(self.sliders[4].value)
        self.sliders[1].dirtyLockValue = False
        self.sliders[4].dirtyLockValue = False

    def keyCommands(self, keyevent):
        if keyevent.key == K_SPACE:
            self.buttonAction(TaskCode.STANDBY)
            
    def getStatusText(self):
        txt = "Running Task: "
        txt = txt + TaskCode.get_task_name(self.current_task_code).upper()
        return txt

    def getSliderValues(self):
        s_vals = []
        for s in self.sliders:
            s_vals.append(s.value)
        return s_vals
    
    def listenToMovement(self):
        #This is a silly stub that pushes some things
        incoming_packet = self.com.get_last_message("movement/defuzzification")
        if incoming_packet:
            vals = incoming_packet["Thruster_Values"]
            A = vals["front_left"]
            B = vals["middle_left"]
            C = vals["back_left"]
            D = vals["back_right"]
            E = vals["middle_right"] 
            F = vals["front_right"]
            self.setThrusterValues(A, B, C, D, E, F)
    
    def getBlankMessage(self):
        return {
                "desired_state": {
                    "task_code": 0, # integer code for each state
                    "name": "standby",
                    "note": "Default Note",
                    "restart": False,
                    "parameter": None
                    }
                }
    
    def publishMessage(self, code):
        msg = self.getBlankMessage()
        msg["desired_state"]["task_code"] = code
        msg["desired_state"]["name"] = TaskCode.get_task_name(code)
        msg["desired_state"]["note"] = "Sent from GUI"
        msg["desired_state"]["restart"] = True
        
        # Params for those whoneed them
        if code == TaskCode.STANDBY_AT_DEPTH:
            msg["desired_state"]["parameter"] = True
        elif code == TaskCode.DIVE:
            msg["desired_state"]["parameter"] = 2. #meters
        else:
            msg["desired_state"]["parameter"] = None
        
        print msg
        self.current_task_code = code
        self.com.publish_message(msg)
        
    def run(self):
        # run the game loop
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
                if event.type == MOUSEBUTTONUP:
                    for b in self.buttons:
                        b.onClickUp(self.buttonAction)
                        
                if event.type == MOUSEBUTTONDOWN:
                    for b in self.buttons:
                        b.onClickDown(event.pos)
                if event.type == KEYDOWN:
                    self.keyCommands(event)

            #handle lock
            if self.dive_lock:
                self.handleLockedSliders()

            self.listenToMovement()
            
            self.drawOther()
            pygame.display.update()
            
            
            

if __name__ == "__main__":
    sd = SliderDebugger()
    sd.run()





