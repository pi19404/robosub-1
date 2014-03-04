# simple button for slider controller

import pygame
from pygame.locals import *
import os

class MySimpleButton():
    def __init__(self, window, pos, size,  txt, uniqueColor, ButtonCode=-1, imgpath=None):
        self.selected = False
        self.BUTTONCODE = ButtonCode
        self.imgpath = imgpath
        self.pos = pos
        self.size = size
        self.window = window
        self.colorSplash =  uniqueColor 
        self.WHITE = (255,255,255)
        self.buttonColor = (50,50, self.colorSplash)
        self.txt = txt

        #config text
        self.basicFont = pygame.font.SysFont("monospace", 22)
        self.text = self.basicFont.render(self.txt, True,  self.WHITE, self.buttonColor)

        #construct
        self.button_rect = self.text.get_rect()
        self.button_rect.left, self.button_rect.top = self.pos
        self.button_rect.width, self.button_rect.height = self.size
        
        if imgpath:
            try:
                self.button_img = pygame.image.load(os.path.join('data', imgpath))
            except:
                print "RUH ROH! Image " + str(imgpath) + " Doesn't exist!!!"
            
    def draw(self):
        self.text = self.basicFont.render(self.txt, True,  self.WHITE, self.buttonColor)
        pygame.draw.rect(self.window, self.buttonColor, self.button_rect, 0)
        self.window.blit(self.text, self.button_rect)

    def onClickDown(self, pos,*largs):
        if self.button_rect.collidepoint(pos):
            self.selected = True
            self.buttonColor = (150,150, self.colorSplash)

    def onClickUp(self, callback, *largs):
        if self.selected:
            callback(self.BUTTONCODE)
        self.selected = False
        self.buttonColor = (50,50, self.colorSplash)
