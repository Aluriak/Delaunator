# COMPILATION
PYTHON_CC=python3.2
# PATHS
PATH_LIB=delaunator/
PATH_PY=pydelaunator/
PATH_PY_LIB=$(PATH_PY)delaunator/
# NAMES
NAME_LIBRARY=delaunator
NAME_LIB=$(PATH_LIB)_$(NAME_LIBRARY).so
NAME_MDL=$(PATH_LIB)$(NAME_LIBRARY).py
NAME_PY=$(PATH_PY)main.py



#########################
# SYSTEM		#
#########################
all: clear do_lib

do_lib: 
	# Generate lib
	cd $(PATH_LIB) && $(MAKE) 
	# move module in py lib directory
	mkdir -p $(PATH_PY_LIB)
	cp $(NAME_LIB) $(PATH_PY_LIB)
	cp $(NAME_MDL) $(PATH_PY_LIB)



py:
	# pythonic python !
	$(PYTHON_CC) $(NAME_PY)






#########################
# OTHER COMMANDS	#
#########################
clean:
	- rm *.o
	- rm -r $()__pycache__/
clear: clean




