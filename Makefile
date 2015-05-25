########################
# BUILD & INSTALL
########################
all: build install

build: clear
	python3 setup.py build

install:
	python3 setup.py install





########################
# OTHERS
########################
src:
	virtualenv ./ --system-site-packages

deac:
	- rm -r lib include bin

upload: clear
	python3 setup.py sdist upload


gendoc:
	#mkdir -p doc/delaunator doc/libdelaunator
	doxygen Doxyfile

updoc:
	# see setup.cfg for exact configuration
	python3 setup.py upload_docs


clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp
	- rm -r dist build delaunator.egg-info
	- rm -r __pycache__/ *.pyc

