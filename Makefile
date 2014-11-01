########################
# BUILD & INSTALL
########################
all: build install 

build: clear
	python3.2 setup.py build

install:
	python3.2 setup.py install





########################
# OTHERS
########################
src:
	virtualenv ./ --system-site-packages

deac:
	- rm -r lib include bin

upload: clear
	python3.2 setup.py sdist upload


gendoc:
	#mkdir -p doc/delaunator doc/libdelaunator
	doxygen Doxyfile

updoc:
	python3.2 setup.py upload_docs --upload-dir=doc/code/html/ --repository=https://pythonhosted.org/delaunator

zipdoc:
	zip doc_zipped.zip doc/code/html/*

clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r dist build delaunator.egg-info 
	- rm -r __pycache__/ *.pyc 

