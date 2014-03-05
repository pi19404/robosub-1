"""Written by Dan Kearney, Natalie Mattison, and Theodore Thompson
for Olin College Computational Modeling 2011."""

import pygame
import numpy as np
import math

class MySlider():
    def __init__(self, (x,y), window, size, color_pos, color_neg, color_norm, rotation):
        self.pos = (x,y)
        self.window = window
        self.size = size
        self.color_pos = color_pos
        self.color_neg = color_neg
        self.color_norm = color_norm
        self.rotation = rotation
        self.WHITE = (255, 255, 255)
        self.RED = (255, 0, 0)
        self.GREEN = (0, 255, 0)
        self.BLUE = (0, 0, 255)

        self.slider_size = (size[0], size[1]/5)
        self.slider_pos = (self.pos[0], self.pos[1]+4*self.size[1]/5)
        self.slider_rect = pygame.Rect(self.slider_pos, self.slider_size)

        self.bar_size = (size[0]/20, size[1]/5)
        self.bar_pos = (self.pos[0]+self.size[0]/2-size[0]/40, self.pos[1]+4*self.size[1]/5)
        self.bar_rect = pygame.Rect(self.bar_pos, self.bar_size)
        
        self.value = 0.0
        self.selected = False
        self.lock = False
        self.dirtyLockValue = False

        self.basicFont = pygame.font.SysFont("monospace", 20)
        self.text = self.basicFont.render(str(self.value), True, self.WHITE, (255, 0, 0))
        self.textRect = self.text.get_rect()
        self.textRect.centerx = self.slider_pos[0] + self.slider_size[0]/2
        self.textRect.centery = self.slider_pos[1] + self.slider_size[1] - 20
        pygame.draw.rect(self.window, (200,200,200), self.textRect, 0)

        #thruster rectangle
        self.thruster_poly = np.array([(self.pos[0]+self.size[0]/3,self.pos[1]+self.size[1]/6),
                              (self.pos[0]+2*self.size[0]/3,self.pos[1]+self.size[1]/6),
                              (self.pos[0]+2*self.size[0]/3,self.pos[1]+2*self.size[1]/3),
                              (self.pos[0]+self.size[0]/3,self.pos[1]+2*self.size[1]/3)])

        #Find the center of the whole item (adjusted one 12th above true center to be center of "thruster"
        self.center = np.array([self.pos[0]+self.size[0]/2,self.pos[1]+self.size[1]/2-self.size[1]/12])

        #rotate points accuratly
        self.thruster_poly = self.thruster_poly - self.center
        self.rotation_matrix = np.array([[math.cos(rotation),math.sin(rotation)],
                                         [-math.sin(rotation), math.cos(rotation)]])
        self.thruster_poly = np.dot(self.rotation_matrix,np.transpose(self.thruster_poly))
        self.thruster_poly = np.transpose(self.thruster_poly) + self.center

        # inner trangle
        self.triangle = np.array([(self.pos[0]+self.size[0]/3,self.center[1]),
                                  (self.pos[0]+2*self.size[0]/3,self.center[1]),
                                  (self.center[0],self.center[1]-30)])

        #rotate inner triangle
        self.triangle = self.triangle - self.center
        self.triangle = np.dot(self.rotation_matrix,np.transpose(self.triangle))
        self.triangle = np.transpose(self.triangle) + self.center
        self.triangle_color = self.BLUE

        #set first frame
        self.onValueChange()
        self.draw()
        
    def draw(self):
        pygame.draw.rect(self.window, self.color_norm, self.slider_rect, 0)

        pygame.draw.rect(self.window, (200,200,200), self.bar_rect, 0)
        #render trhust value correctly
        self.text = self.basicFont.render("{0:.2f}".format(round(self.value,2)),
                                          True, self.WHITE, (255, 0, 0))
        pygame.draw.rect(self.window, (200,200,200), self.textRect, 0)

        pygame.draw.lines(self.window,(255,0,0),True,self.thruster_poly,3)
        
        pygame.draw.polygon(self.window,self.triangle_color,self.triangle,0)
        
        self.window.blit(self.text, self.textRect)

    def onClickDown(self, pos,*largs):
        if self.slider_rect.collidepoint(pos):
            self.selected = True
            self.onMouseMove(pos)
                    
    def onClickUp(self, *largs):
        self.selected = False
            

    def onMouseMove(self,pos):
        if self.selected:
            if pos[0] < self.slider_pos[0]:
                self.value = -1.0
            elif pos[0] > self.slider_pos[0]+self.slider_size[0]:
                self.value = 1.0
            else:
                self.value = (((pos[0]-self.slider_pos[0]) / float(self.slider_size[0])) - 0.5) / 0.5

            self.onValueChange()

    def setValue(self, newValue):
        if abs(newValue) <= 1.0:
            self.value = newValue
            self.onValueChange()

    def setLock(self):
        self.lock = not self.lock
        

    def onValueChange(self):
         #round value!
         self.value = round(self.value,2)
        
         #Change text
         self.text = self.basicFont.render(str(self.value), True, self.WHITE, self.WHITE)

         #move text
         self.bar_rect.left,  self.bar_rect.top = \
                ( self.pos[0]+(self.size[0]/2-self.size[0]/40)+self.value*self.size[0]/2,
                 self.pos[1]+4*self.size[1]/5 )
         #move textRect
         self.textRect.centerx = self.bar_rect.left

         #adjust trangle
         triangle_point = np.array([0,(self.size[1]/2)*self.value/2.])
         self.triangle[2] = np.dot(self.rotation_matrix,triangle_point) + self.center

         #set triangle color
         if self.value == 0:
             self.triangle_color = self.BLUE
         elif self.value > 0:
             self.triangle_color = self.GREEN
         else:
             self.triangle_color = self.RED

         if self.lock:
             self.dirtyLockValue = True
      

            
            
        
        





















        
