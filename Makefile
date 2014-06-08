all: clear
	python3.2 setup.py install

src:
	virtualenv ./ --system-site-packages
	source bin/activate

deac:
	- deactivate
	- rm -r lib include bin

upload: clear
	python3.2 setup.py sdist upload


gendoc:
	#mkdir -p doc/delaunator doc/libdelaunator
	doxygen Doxyfile

clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r delaunator/libdelaunator_src/libdelaunator.py 
	- rm -r dist build delaunator.egg-info 
	- rm -r __pycache__/ *.pyc 

