all: clear
	python3.2 setup.py install

src:
	source bin/activate

upload: clear
	python3.2 setup.py sdist upload

clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r dist build delaunator.egg-info 

