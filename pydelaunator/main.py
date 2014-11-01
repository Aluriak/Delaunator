# -*- coding: utf-8 -*-
#########################
#       LINK            #
#########################


#########################
# IMPORTS               #
#########################
from delaunator import Delaunator, VERTEX_FINDER_MODE_RANDOM
from delinterface.delinterface import Delinterface
from random import randint
from commons.commons import UNIVERSE_DEFAULT_SIZE



#########################
# PRE-DECLARATIONS      #
#########################


#########################
# MAIN                  #
#########################
if __name__ == '__main__':
    dt = Delaunator(0,600,0,600, VERTEX_FINDER_MODE_RANDOM)

    gui = Delinterface(dt)
    
    gui.join()



