# -*- coding: utf-8 -*-
#########################
#   DELINTERFACE        #
#########################


#########################
# IMPORTS               #
#########################
from delaunator import Delaunator, Coordinates, TrianguledObject
from commons.commons import INTERFACE_TIME_SPEED, VIDEO_MODE_X, VIDEO_MODE_Y, PROGRAM_NAME, MOUSE_PRECISION
from random import randint, choice
from threading import Thread
import pygame
import time
import math





#########################
# PRE-DECLARATIONS      #
#########################
class Point(TrianguledObject):
    def __init__(self, color=None):
        super().__init__()
        if color is None:
            color = (randint(1, 255), randint(1, 255), randint(1, 255))
        self.color = color



#########################
# DELINTERFACE          #
#########################
class Delinterface(Thread):
    """
    Graphical representation of a delaunator triangulation.
    Commands:
        ESC: quit
        ESPACE or ENTER: add a Point at mouse position
        DELETE: remove Point at mouse position
        drag&drop: on Point for move it
        a: toggle auto-adding of Point
        i: move all Point randomly
        ARROWS: movement of 500 px on dragged Point

    Color & Shape code:
        - each Point have his own color, show by the circle around the point
        - each edge that rely Point are green lines
        - when two or more Point at the same location, there are two circles around a Point
        - in drag&drop mode:
            - edges that go to direct neighbors are in red.
            - two black circles around the dragged Point mark distance from 150 to 200 pixels
            - all Point contained in these limits have an additionnal black circle
            - the seven nearer Point are circled in blue
    """

# CONSTRUCTOR #################################################################
    def __init__(self, dt):
        """"""
        # delaunay triangulation
        self.dt = dt

        # states
        self.finish = False
        self.auto_add = False
        self.print_neighbors = True
        self.dragging = False
        self.dragged_point = None

        # pygame
        Thread.__init__(self)
        pygame.display.init()

        self.screen = pygame.display.set_mode((VIDEO_MODE_X, VIDEO_MODE_Y), pygame.DOUBLEBUF | pygame.HWSURFACE)
        pygame.display.set_caption(PROGRAM_NAME)
        self.start()


    def __del__(self):
        pygame.display.quit()



# PUBLIC METHODS ##############################################################
    def run(self):
        """Start priniting of engine simulation"""
        print("WANDA JACKSON")
        print(Delinterface.__doc__)

        # just for begin…
        for i in range(4):
            self.addPointToDT()

        while not self.finish:
            # auto adding
            if self.auto_add:
                self.addPointToDT()
            # printing
            self.screen.fill((255,255,255))
            self.draw()
            pygame.display.flip()
            # waiting
            time.sleep(INTERFACE_TIME_SPEED)

            # event looping
            for event in pygame.event.get():

                # END CASE
                if event.type == pygame.QUIT:
                    self.finishing()

                # KEYDOWN
                if event.type == pygame.KEYDOWN:
                    # SPECIAL KEY HITING
                    if event.key == pygame.K_ESCAPE:    self.finishing()
                    # add new point
                    elif event.key == pygame.K_RETURN or event.key == pygame.K_SPACE:
                        x, y = pygame.mouse.get_pos()
                        self.addPointToDT(x, y)
                    # delete point
                    elif event.key == pygame.K_DELETE: 
                        point = self.getPointAt(*pygame.mouse.get_pos())
                        if point is not None:
                            self.delPoint(point)

                    # ARROW HITING
                    elif event.key == pygame.K_UP:      self.movePoint(0,    -500, self.dragged_point)
                    elif event.key == pygame.K_RIGHT:   self.movePoint(500,  0,    self.dragged_point)
                    elif event.key == pygame.K_DOWN:    self.movePoint(0,    500,  self.dragged_point)
                    elif event.key == pygame.K_LEFT:    self.movePoint(-500, 0,    self.dragged_point)
                    # OPTIONS HITING
                    elif event.key == pygame.K_a:       self.auto_add = not self.auto_add
                    elif event.key == pygame.K_n:       self.print_neighbors = not self.print_neighbors
                    elif event.key == pygame.K_i:       self.moveAllPoints()

                # MOUSE BUTTON
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.dragged_point = self.getPointAt(*pygame.mouse.get_pos())
                    if self.dragged_point is not None: self.dragging = True

                elif event.type == pygame.MOUSEBUTTONUP:
                    self.dragging = False
                    self.dragged_point = None

                # MOUSE MOTION
                elif event.type == pygame.MOUSEMOTION:
                    x,y = pygame.mouse.get_rel()
                    if self.dragging:
                        self.movePoint(x, y, self.dragged_point)

        print("WANDA JACKSOFF")




    ################# END METHODS #################
    def finishing(self):
        """turn off simulation"""
        self.finish = True




    ################# DRAW METHODS #################
    def draw(self):
        """Print current state of engine"""
        for obj in self.dt.trianguledObjects():

            # shortcuts
            line_width, circle_width = 1, 2
            line_color, circle_color = (0, 255, 0), obj.color
            diameter = 20
            p2 = obj.coordinates()

            # print neighbors when obj is dragged point
            if self.print_neighbors and obj == self.dragged_point: 
                line_color = (255, 0, 0) 
                line_width = 2

                # print all points that are between 200 and 150 px of distance
                pygame.draw.circle(self.screen, (0, 0, 0), (int(p2.x), int(p2.y)), 200, circle_width)
                pygame.draw.circle(self.screen, (0, 0, 0), (int(p2.x), int(p2.y)), 150, circle_width)
                for cnei in obj.neighborsAt(200, 150):
                    p1 = cnei.coordinates()
                    pygame.draw.circle(self.screen, (0, 0, 0), (int(p1.x), int(p1.y)), 20, circle_width*2)

                # print the 7 nearer neighbors
                for cnei in obj.nearerNeighbors(7): 
                    p1 = cnei.coordinates()
                    pygame.draw.circle(self.screen, (0, 0, 255), (int(p1.x), int(p1.y)), int(diameter/2)+1, 5)

            
            # print confunded neighbors circle, only if its id is lower
            for cnei in obj.confundedNeighbors:
                p1 = cnei.coordinates()
                pygame.draw.circle(self.screen, cnei.color, (int(p1.x), int(p1.y)), int(diameter), circle_width)



            # printing !
            pygame.draw.circle(self.screen, obj.color, (int(p2.x), int(p2.y)), int(diameter/2), circle_width)
            for cnei in obj.directNeighbors:
                p1 = cnei.coordinates()
                pygame.draw.line(self.screen, line_color, (p1.x, p1.y), (p2.x, p2.y), line_width)





    
    ################# POINT METHODS #################
    def addPointToDT(self, x = None, y = None):
        """Add a random point in DT"""
        # creat Coordinates
        coords = None
        if x is None or y is None:
            coords = Coordinates(randint(self.dt.getXmin(), self.dt.getXmax()), randint(self.dt.getYmin(), self.dt.getYmax()))
        else:
            coords = Coordinates(x, y)
        # creat color
        color = (randint(1, 255), randint(1, 255), randint(1, 255))
        # creat object
        self.dt.addTrianguledObject(Point(color), coords)


    def delPoint(self, point):
        """Delete given Point"""
        if self.dragged_point == point:
            self.dragged_point = None
            self.dragging = False
        self.dt.delTrianguledObject(point)


    def getPointAt(self, x, y):
        """Return point that is at given coordinates (about MOUSE_PRECISION)"""
        return TrianguledObject.of(self.dt.virtualVertexAt(x, y, MOUSE_PRECISION))

    
    def movePoint(self, x, y, p=None):
        """Add given values to (x;y) of last added point"""
        if p is not None:
            self.dt.movTrianguledObject(p, (x, y))


    def moveAllPoints(self):
        """Move all vertices by a small move"""
        # use of virtual vertice in place of trianguled vertice
        # because don't need trianguled objects data, and this is a little quicker to deal with VirtualVertex directly
        for v in self.dt.virtualVertices():
            eps = self.dt.epsilon()
            mx = choice([-eps,eps])
            my = choice([-eps,eps])
            self.dt.movVirtualVertex(v, mx, my)




# PRIVATE METHODS #############################################################
# PREDICATS ###################################################################
# ACCESSORS ###################################################################
# CONVERSION ##################################################################
# OPERATORS ###################################################################





