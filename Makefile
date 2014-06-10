all: clear
	python3.2 setup.py install

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
	#python3.2 setup.py upload_docs --upload-dir=doc/html/ --repository=https://pythonhosted.org/delaunator
	zip doc_zipped.zip doc/html/*

clear:
	- rm -r delaunator/libdelaunator_src/libdelaunator_wrap.cpp 
	- rm -r dist build delaunator.egg-info 
	- rm -r __pycache__/ *.pyc 

