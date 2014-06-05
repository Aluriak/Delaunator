all: clear
	python3.2 setup.py install

source:
	source bin/activate


clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r dist build delaunator.egg-info 

