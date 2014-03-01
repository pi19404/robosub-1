# SLIDER GUI DEBUGGER

import pygame, sys
from pygame.locals import *


class SliderDebugger():
    """
    INFO!
    """
    def __init__(self, screenSize=(500, 400)):
        # set up pygame
        pygame.init()

        # set up the window
        self.windowSurface = pygame.display.set_mode(screenSize, 0, 32)
        pygame.display.set_caption('Hello world!')
        self.windowSize = screenSize
        # set up the colors
        self.BLACK = (0, 0, 0)
        self.WHITE = (255, 255, 255)
        self.RED = (255, 0, 0)
        self.GREEN = (0, 255, 0)
        self.BLUE = (0, 0, 255)
        # set up fonts

        print "Attempting to initialize Font (if I fail, this is probably what went wrong!!)"
        try:
            self.basicFont = pygame.font.SysFont("monospace", 15)
        except:
            print "RUH ROH! Font type problem!  I probably crashed :("

        if(self.basicFont):
            print "Font Initialized!"

        # set up the text
        self.text = self.basicFont.render('Hello world!', True, self.WHITE, self.BLUE)
        self.textRect = self.text.get_rect()
        self.textRect.centerx = self.windowSurface.get_rect().centerx
        self.textRect.centery = self.windowSurface.get_rect().centery

        # draw the white background onto the surface
        self.windowSurface.fill(self.WHITE)

        # draw a green polygon onto the surface
        pygame.draw.polygon(self.windowSurface, self.GREEN, ((146, 0), (291, 106), (236, 277), (56, 277), (0, 106)))

        # draw some blue lines onto the surface
        pygame.draw.line(self.windowSurface, self.BLUE, (60, 60), (120, 60), 4)
        pygame.draw.line(self.windowSurface, self.BLUE, (120, 60), (60, 120))
        pygame.draw.line(self.windowSurface, self.BLUE, (60, 120), (120, 120), 4)

        # draw a blue circle onto the surface
        #pygame.draw.circle(windowSurface, self.BLUE, (300, 50), 20, 0)

        # draw a red ellipse onto the surface
        #pygame.draw.ellipse(windowSurface, self.RED, (300, 250, 40, 80), 1)

        # draw the text's background rectangle onto the surface
        pygame.draw.rect(self.windowSurface, self.RED, (self.textRect.left - 20, self.textRect.top - 20, self.textRect.width + 40, self.textRect.height + 40))

        # get a pixel array of the surface
        #pixArray = pygame.PixelArray(windowSurface)
        #pixArray[480][380] = BLACK
        #del pixArray

        # draw the text onto the surface
        self.windowSurface.blit(self.text, self.textRect)

        # draw the window onto the screen
        pygame.display.update()
        
    def drawGrid(self, squareSize, color):
        """
        Draws a centered grid of size specified
        """
        h,w = self.windowSize
        #draw horizontal lines
        pygame.draw.line(self.windowSurface, color, start_pos, end_pos, width=1) -> Rect
        
    
    def run(self):
        # run the game loop
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    pygame.quit()
                    sys.exit()
            pygame.display.update()


sd = SliderDebugger()
sd.run()
