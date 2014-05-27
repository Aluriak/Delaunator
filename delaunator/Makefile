# ARGUMENTS
DEBUG=yes
DOLIB=yes
WRAPR=yes
OPTMS=ye
# COMPILATION
FLAGS_LK_LIB=-lm -std=c++11
FLAGS=-Wall -Werror $(FLAGS_LK_LIB) 
FLAGS_LK=-Wall -Werror $(FLAGS_LK_LIB) 
COMPILER=g++
SWIG=swig2.0
TARGET_WRAPPER=python
# PATHS
PATH_LIB_SRC=libdelaunator_src/
PATH_LIB=./
# NAMES
NAME_LIBRARY=libdelaunator
NAME_ELF=a.out
NAME_LIB=$(NAME_LIBRARY).so

#########################
# ARGUMENTS		#
#########################
ifeq ($(OPTMS), yes)
	OPT_OPTIMIZATION=-O2 -flto
	SWIG_OPT=-O -Wall
endif
ifeq ($(DEBUG), yes)
	OPT_DEB=-D DEBUG
endif
ifeq ($(DOLIB), yes)
	OPT_LIB=-fpic -shared $(OPT_OPTIMIZATION)
	NAME=$(NAME_LIB)
	MAIN_TARGET=delaunator.o
    ifeq ($(WRAPR), yes)
	    PATH_PYTHON_H=-I/usr/include/python3.2
	    DO_WRAPPER=wrapper
	    NAME_LIB=_$(NAME_LIBRARY).so
    endif
else
	NAME=$(NAME_ELF)
	MAIN_TARGET=main.o
endif



#########################
# SYSTEM		#
#########################
all: clear $(DO_WRAPPER) compile linking

compile: $(MAIN_TARGET)

linking:
	$(COMPILER) $(FLAGS_LK) $(PATH_PYTHON_H) $(OPT_DEB) $(OPT_LIB) *.o -o $(PATH_LIB)$(NAME)
	# delete objects
	rm *.o
	# bring here the py module
	mv $(PATH_LIB_SRC)$(NAME_LIBRARY).py ./

elf:
	$(PATH_LIB)a.out

wrapper: 
	# Creat wrapper for python
	$(SWIG) $(SWIG_OPT) -$(TARGET_WRAPPER) -c++ $(PATH_LIB_SRC)$(NAME_LIBRARY).i
	# compile lib wrapper
	$(COMPILER) $(FLAGS_LK_LIB) $(OPT_LIB) $(PATH_PYTHON_H) -c $(PATH_LIB_SRC)$(NAME_LIBRARY)_wrap.cxx



#########################
# SOURCES		#
#########################
main.o:	delaunator.o
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)main.cpp

delaunator.o: utils.o edge.o
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)delaunator.cpp

edge.o: vertex.o face.o
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)edge.cpp
vertex.o: 
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)vertex.cpp
face.o: 
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)face.cpp
utils.o: 
	$(COMPILER) $(FLAGS) $(OPT_DEB) $(OPT_LIB) -c $(PATH_LIB_SRC)utils.cpp





#########################
# OTHER COMMANDS	#
#########################
clean: clear
clear: 
	- rm $(PATH_LIB)$(NAME_LIB)
	- rm $(PATH_LIB)$(NAME_ELF)
	- rm -r *.o *.py *.pyc *.so __pycache__/
	- rm -r $(PATH_LIB)*.o $(PATH_LIB)*_wrap.cxx $(PATH_LIB)*.py $(PATH_LIB)*.pyc $(PATH_LIB)*.so $(PATH_LIB)__pycache__/


clean:
