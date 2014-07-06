# -*- coding: utf-8 -*-
#########################
#       LINK            #
#########################


#########################
# IMPORTS               #
#########################
from delaunator import Delaunator
from delinterface.delinterface import Delinterface
from random import randint



#########################
# PRE-DECLARATIONS      #
#########################


#########################
# MAIN                  #
#########################
if __name__ == '__main__':
    dt = Delaunator(0,600,0,600, Delaunator.FINDER_INITIAL_RANDOM)

    gui = Delinterface(dt)
    
    gui.join()



