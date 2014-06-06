all: clear
	python3.2 setup.py install

src:
	virtualenv ./ --system-site-packages
	source bin/activate

deac:
	deactivate

upload: clear
	python3.2 setup.py sdist upload

clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r dist build delaunator.egg-info 

