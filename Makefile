# ARGUMENTS
DEBUG=yes
#DOLIB=
#WRAPR=
DOLIB=yes
WRAPR=yes
OPTMS=
# COMPILATION
FLAGS_LK_LIB=-lm -std=c++11
FLAGS=-Wall -Werror $(FLAGS_LK_LIB) 
FLAGS_LK=-Wall -Werror $(FLAGS_LK_LIB) 
COMPILER=g++
PYTHON_CC=python3.2
# PATHS
PATH_LIB_SRC=src/libdelaunator/libdelaunator_src/
PATH_LIB=src/libdelaunator/
# NAMES
NAME_LIBRARY=libdelaunator
NAME_ELF=a.out
NAME_LIB=$(NAME_LIBRARY).so
NAME_PY=src/main.py

#########################
# ARGUMENTS		#
#########################
ifeq ($(OPTMS), yes)
	OPT_OPTIMIZATION=-O2 -flto
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
	rm *.o

py:
	# move files and change final lib name
	cp $(PATH_LIB_SRC)$(NAME_LIBRARY).py $(PATH_LIB)
	# pythonic python !
	$(PYTHON_CC) $(NAME_PY)

elf:
	$(PATH_LIB)a.out

wrapper: 
	# delete wrapper object file
	-rm $(NAME_LIBRARY)_wrap.o
	# Creat wrapper for python
	swig2.0 -python -c++ $(PATH_LIB_SRC)$(NAME_LIBRARY).i 
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
clean:
	- rm *.o
	- rm -r __pycache__/
clear: clean
	- rm $(PATH_LIB)$(NAME_LIB)
	- rm $(PATH_LIB)$(NAME_ELF)




