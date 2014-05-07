# -*- coding: utf-8 -*-
#########################
#       LINK            #
#########################


#########################
# IMPORTS               #
#########################
from libdelaunator.libdelaunator import Delaunator
from delinterface.delinterface import Delinterface
from random import randint



#########################
# PRE-DECLARATIONS      #
#########################


#########################
# MAIN                  #
#########################
if __name__ == '__main__':
    dt = Delaunator(0,600,0,600)


    gui = Delinterface(dt)
    ##tabt = []
    ##for i in range(dt.lstTri.nbItem):
        ##t = TL_getTri(dt.lstTri, i)
        ##tabt.append(t)
    ##print(tabt)
    
    gui.join()



