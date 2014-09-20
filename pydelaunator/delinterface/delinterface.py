# -*- coding: utf-8 -*-
#########################
#   DELINTERFACE        #
#########################


#########################
# IMPORTS               #
#########################
from delaunator import Delaunator, Vertex, Coordinates, Edge
from delaunator import IteratorOnAllEdges, IteratorFaceToNeighbourFaces, IteratorVertexToNeighbourVertices
from commons.commons import INTERFACE_TIME_SPEED, VIDEO_MODE_X, VIDEO_MODE_Y, PROGRAM_NAME, OFFSET, MOUSE_PRECISION
from random import randint, choice
from threading import Thread
import pygame
import time
import math





#########################
# PRE-DECLARATIONS      #
#########################



#########################
# CLASS                 #
#########################
class Delinterface(Thread):
    """"""

# CONSTRUCTOR #################################################################
    def __init__(self, dt):
        """"""
        Thread.__init__(self)
        pygame.display.init()
        self.dt = dt
        self.finish = False
        self.pause  = False
        self.auto_add = False
        self.print_neighbors = False
        self.print_triangles = True
        self.dragging = False
        self.screen = pygame.display.set_mode((VIDEO_MODE_X, VIDEO_MODE_Y), pygame.DOUBLEBUF | pygame.HWSURFACE)
        pygame.display.set_caption(PROGRAM_NAME)
        self.start()

    def __del__(self):
        pygame.display.quit()



# PUBLIC METHODS ##############################################################
    def run(self):
        """Start priniting of engine simulation"""
        print("WANDA JACKSON")
        while not self.finish:
            if self.auto_add:
                self.addPointToDT()
            if not self.pause:
                self.screen.fill((255,255,255))
                self.draw()
                pygame.display.flip()
            # wait some time: graphical FPS
            time.sleep(INTERFACE_TIME_SPEED)
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.finishing()
                if event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        self.finishing()
                    elif event.key == pygame.K_RETURN:
                        x, y = pygame.mouse.get_pos()
                        self.addPointToDT(x-OFFSET, y-OFFSET)
                    elif event.key == pygame.K_SPACE:
                        self.togglePause()
                    elif event.key == pygame.K_DELETE:
                        vertex = self.getPointAt(*pygame.mouse.get_pos())
                        if vertex is not None:
                            self.dt.delVertex(vertex)
                    elif event.key == pygame.K_UP:
                        self.movePoint(0,-500,self.dragged_point)
                    elif event.key == pygame.K_RIGHT:
                        self.movePoint(500,0,self.dragged_point)
                    elif event.key == pygame.K_DOWN:
                        self.movePoint(0,500,self.dragged_point)
                    elif event.key == pygame.K_LEFT:
                        self.movePoint(-500,0,self.dragged_point)
                    elif event.key == pygame.K_a:
                        self.auto_add = not self.auto_add
                    elif event.key == pygame.K_n:
                        self.print_neighbors = not self.print_neighbors
                    elif event.key == pygame.K_t:
                        self.print_triangles = not self.print_triangles
                    elif event.key == pygame.K_i:
                        self.moveAllVertices()
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.dragged_point = self.getPointAt(*pygame.mouse.get_pos())
                    if self.dragged_point is not None:
                        self.dragging = True
                elif event.type == pygame.MOUSEBUTTONUP:
                    self.dragging = False
                    self.dragged_point = None
                elif event.type == pygame.MOUSEMOTION:
                    x,y = pygame.mouse.get_rel()
                    if self.dragging:
                        self.movePoint(x, y, self.dragged_point)
        print("WANDA JACKSOFF")


    def getPointAt(self, x, y):
        """Return point that is at given coordinates (about MOUSE_PRECISION)"""
        return self.dt.vertexAt(x, y, MOUSE_PRECISION)

    
    def togglePause(self, activePause=None):
        """activePause : bool
        return : new state of pause
        If ommitted argument, pause is toggle, else its set to given value"""
        if activePause is None: self.pause = not self.pause
        else:                   self.pause = activePause
        return self.pause


    def setpause(self): self.togglePause(True)
    def unpause(self): self.togglePause(False)


    def finishing(self):
        """turn off simulation"""
        self.setpause()
        self.finish = True


    def draw(self):
        """Print current state of engine"""
        #print("DRAWING")
        for it in self.dt.iterEdges():
            p1 = it.originVertex()
            p2 = it.destinVertex()
            if it.isVisible():
                if self.dragging and self.dragged_point == p1:
                    color = (255,0,0)
                    width = 3
                elif self.dt.opt_follow_search() and  it.passing:
                    color = (0,0,255)
                    width = 4
                else:
                    color = (0,255,0)
                    width = 1
            else:
                color = (0,0,200)
                width = 1
            pygame.draw.line(self.screen, color, (p1.x+OFFSET, p1.y+OFFSET), (p2.x+OFFSET, p2.y+OFFSET), width)
            pygame.draw.circle(self.screen, color, (int(p2.x+OFFSET), int(p2.y+OFFSET)), 10, width)





    
    def movePoint(self, x, y, p=None):
        """Add given values to (x;y) of last added point"""
        if p is not None:
            self.dt.moveVertex(p, x, y)


    def moveAllVertices(self):
        """Move all vertices by a small move"""
        for v in self.dt.allVertices():
            mx = choice([-0.01,0.01])
            my = choice([-0.01,0.01])
            self.dt.moveVertex(v, mx, my)

    def addPointToDT(self, x = None, y = None):
        """Add a random point in DT"""
        p = None
        if x is None or y is None:
            p = Coordinates(randint(self.dt.getXmin(), self.dt.getXmax()), randint(self.dt.getYmin(), self.dt.getYmax()))
        else:
            p = Coordinates(x, y)
        self.dt.addVertexAt(p)

# PRIVATE METHODS #############################################################
# PREDICATS ###################################################################
# ACCESSORS ###################################################################
# CONVERSION ##################################################################
# OPERATORS ###################################################################





#########################
# FUNCTIONS             #
#########################
#pygame.init()

#class Hangman():
    #def __init__(self):
        #self.lines = 0 #Number of lines to be drawn

    #def hang(self):
        #self.lines += 1

    #def draw(self,screen):
        #for x in range(self.lines):
            #coord1 = (x*10,20)
            #coord2 = (x*10,50)
            #pygame.draw.line(screen,(0,0,0),coord1,coord2)

#size = screenWidth,screenHeight = 200,70
#screen = pygame.display.set_mode(size)
#pygame.display.flip()

#myman = Hangman()

#drawlist = []
#drawlist.append(myman)
##mainloop
#running = True
#while running:
    ##EVENT HANDLING#
    #for event in pygame.event.get():
        #if event.type == pygame.QUIT:
            #running = False
        #if event.type == pygame.KEYDOWN:
            #if event.key == pygame.K_SPACE: #Spacebar
                #myman.hang()
            #elif event.key == pygame.K_ESCAPE:
                #running = False

    ##DRAWING#
    #screen.fill((255,255,255))
    #for item in drawlist:
        #item.draw(screen)
    #pygame.display.flip()








#########################
# FUNCTIONS             #
#########################



