# -*- coding: utf-8 -*-
"""
Graphical representation of a delaunator triangulation.
Commands:
    ESC: quit
    ESPACE or ENTER: add a Point at mouse position
    DELETE: remove Point at mouse position
    drag&drop: on Point for move it
    a: toggle auto-adding of Point
    t: toggle random moving
    s: take a snapshot of all points positions
    ARROWS: movement of 500 px on dragged Point [NOT IMPLEMENTED]

"""


#########################
# IMPORTS               #
#########################
from delaunator      import Delaunator, Coordinates, TrianguledObject
from commons.commons import INTERFACE_TIME_SPEED, VIDEO_MODE_X, VIDEO_MODE_Y
from commons.commons import PROGRAM_NAME, MOUSE_PRECISION
from random          import randint, choice
from pyglet.window   import key
from pyglet.window   import mouse
import itertools
import functools
import pyglet
import time
import math
import sys





#########################
# PRE-DECLARATIONS      #
#########################
class Point(TrianguledObject):
    def __init__(self, color=None):
        super().__init__()
        if color is None:
            color = tuple(randint(1, 255) for _ in range(4))
        self.color = color

# states
auto_add = False
auto_mov = False
dragged_point = None
mouse_position = None
counter = 0
dt = None

# pyglet
window = pyglet.window.Window(
    width=VIDEO_MODE_X, height=VIDEO_MODE_Y,
    caption=PROGRAM_NAME
)

# flags
PRINT_EDGES = True


#########################
# FUNCTIONS             #
#########################
def start_gui(dt_object):
    """dt must be a delaunator object"""
    # first printings
    global dt
    dt = dt_object
    print("WANDA JACKSON")
    print(__doc__)

    # just for begin…
    # for i in range(4):
        # _addPointToDT()
    # UNIT TEST 1
    # _addPointToDT(200, 100)
    # _addPointToDT(100,   0)
    # _addPointToDT(  0, 100)
    # _addPointToDT(100, 200)
    # _addPointToDT(221, 249)
    # _addPointToDT(258, 305)
    # UNIT TEST 2: test the face stacking when simplifying delaunay
    #   triangulation in suppression point algorithm.
    # _addPointToDT(121, 5)
    # _addPointToDT(161, 61)
    # _addPointToDT(211, 135)
    # _addPointToDT(282, 176)
    # _addPointToDT(357, 54) # its d
    # _addPointToDT(508, 253)
    # _addPointToDT(533, 52)
    # UNIT TEST 3
    # _addPointToDT(91.0, 372.0)
    # _addPointToDT(122.0, 309.0)
    # _addPointToDT(118.0, 204.0)
    # _addPointToDT(250.0, 575.0)
    # _addPointToDT(312.0, 107.0)
    # _addPointToDT(387.0, 204.0)
    # _addPointToDT(506.0, 480.0)
    # _addPointToDT(493.0, 216.0)
    # _addPointToDT(308, 373)
    # UNIT TEST 4
    _addPointToDT(75.0, 381.0)
    _addPointToDT(133.0, 192.0)
    _addPointToDT(249.0, 574.0)
    _addPointToDT(320.0, 107.0)
    _addPointToDT(503.0, 488.0)
    _addPointToDT(528.0, 209.0)
    _addPointToDT(465, 325) # deletion cause delaunay condition breaking



    # run pyglet printings
    pyglet.app.run()
    pyglet.app.exit()
    print("\nWANDA JACKSOFF")


@window.event
def on_key_press(symbol, modifiers):
    global auto_add, mouse_position
    if symbol == key.RETURN or symbol == key.SPACE: # add point
        if mouse_position is not None:
            _addPointToDT(*mouse_position)
    elif symbol in (key.A, key.B):
        if auto_add:
            pyglet.clock.unschedule(_addPointToDT)
        else:
            pyglet.clock.schedule_interval(
                _addPointToDT,
                INTERFACE_TIME_SPEED
            )
        auto_add = not auto_add
    elif symbol in (key.M, key.T):
        _moveAllPoints()
    elif symbol in (key.S,):
        _snapshot()
    elif symbol == key.DELETE:  # del point
        if mouse_position is not None:
            _delPoint(_getPointAt(*mouse_position))


@window.event
def on_mouse_motion(x, y, dx, dy):
    global mouse_position, dragged_point
    mouse_position = (x, y)

@window.event
def on_mouse_press(x, y, buttons, modifiers):
    global dt, mouse_position, dragged_point
    mouse_position = (x, y)
    if buttons == pyglet.window.mouse.LEFT:
        dragged_point = _getPointAt(x, y)

@window.event
def on_mouse_release(x, y, buttons, modifiers):
    global mouse_position, dragged_point
    mouse_position = (x, y)
    if buttons == pyglet.window.mouse.LEFT:
        dragged_point = None

    if dragged_point is not None:
        movePoint(dx, dy, dragged_point)

@window.event
def on_mouse_drag(x, y, dx, dy, buttons, modifiers):
    global mouse_position, dragged_point
    mouse_position = (x, y)
    if dragged_point is not None:
        _movePoint(dx, dy, dragged_point)

@window.event
def on_draw():
    global dt, mouse_position, dragged_point
    window.clear()
    _draw_edges()
    # print number of objects in stdout
    print('\r', len(tuple(dt.trianguledObjects())), end='')
    sys.stdout.flush() # print even if line is not finish



################# DRAW METHODS #################
def _draw_edges():
    """Print current state of engine"""
    global dt, dragged_point
    def print_edge(coords, color, width=1):
        pyglet.graphics.draw(2, pyglet.gl.GL_LINES,
                                 ('v2f', coords),
                                 ('c4B', color),
                            ) #TODO: use width


    if not PRINT_EDGES:
        for obj in dt.trianguledObjects():
            pass
    else:
        for obj in dt.trianguledObjects():
            color_green = (0, 255, 0, 255) # GREEN
            color_red   = (255, 0, 0, 255) # RED
            diameter = 20
            p2 = obj.coordinates()
            # printing !
            for cnei in obj.directNeighbors:
                p1 = cnei.coordinates()
                # use red for dragged point
                if obj == dragged_point:
                    line_color = cnei.color + color_red
                elif cnei == dragged_point:
                    line_color = color_red + obj.color
                else: # no point involved is the dragged one
                    line_color = cnei.color + obj.color
                # print
                print_edge(
                    (p1.x, p1.y, p2.x, p2.y),
                    line_color,
                )



################# POINT FUNCTIONS #################
def _addPointToDT(x = None, y = None):
    """Add a random point in DT"""
    global dt
    # creat Coordinates
    coords = None
    if x is None or y is None:
        coords = Coordinates(
            randint(dt.getXmin(), dt.getXmax()),
            randint(dt.getYmin(), dt.getYmax())
        )
    else:
        coords = Coordinates(x, y)
    # creat color & object
    dt.addTrianguledObject(Point(), coords)


def _delPoint(point):
    """Delete given Point"""
    global dt, dragged_point
    if point is not None:
        if dragged_point == point:
            dragged_point = None
        dt.delTrianguledObject(point)


def _getPointAt(x, y):
    """Return point that is at given coordinates (about MOUSE_PRECISION)"""
    global dt
    return dt.trianguledObjectAt((x, y), MOUSE_PRECISION)


def _movePoint(x, y, p):
    """Add given values to (x;y) of given point"""
    global dt
    if p is not None:
        dt.movTrianguledObject(p, (x, y))


def _moveAllPoints():
    """Move all vertices by a small move"""
    global dt, dragged_point
    for v in dt.trianguledObjects():
        eps = 1
        mx = choice([-eps,eps])
        my = choice([-eps,eps])
        if v is not dragged_point:
            dt.movTrianguledObject(v, (mx, my))


def _snapshot():
    global dt
    print('\nsnapshot:')
    for c in (o.coordinates() for o in dt.trianguledObjects()):
        print('\t_addPointToDT' + str((c.x, c.y)))






#########################
# MAIN                  #
#########################
if __name__ == '__main__':
    from delaunator import Delaunator, VERTEX_FINDER_MODE_RANDOM
    # from commons.commons import UNIVERSE_DEFAULT_SIZE
    dt = Delaunator(0, 600, 0, 600, VERTEX_FINDER_MODE_RANDOM)
    start_gui(dt)



