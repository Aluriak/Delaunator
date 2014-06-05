# Delaunator
2D Delaunay Triangulation in C++ with Python wrapper.  
(or wathever you want, thanks to SWIG)   
  
__LICENSE:__ _GPL-2.0_; cf LICENSE.txt file.  
__Portability:__ code and tests on Debian Stable, with c++11, gcc 4.7.2, make 3.81, python 3.2 and SWIG 2.0; i don't know for others platforms, but except for windows, i think portability is totally possible, maybe already done. 


## INSTALLATION
On debian, SWIG is named *swig2.0*, and that is problematic, because setuptools use *swig*.
So, you can say to setuptools to use swig2.0 instead, with command *python3 setup.py setopt --command=build_ext --option=swig --set-value=swig2.0*.
Another way to solve that is to creat an alias in /usr/bin with *ln -s /usr/bin/swig2.0 /usr/bin/swig*.




## PROGRADE DIRECTION
- [x] do triangulation; 
- [x] print it by Python script and pygame; 
- [x] do delaunay triangulation; 
- [x] manage movement;  
- [x] do a victory cry;  
- [ ] find a better way than TTL limit for fix infinite recursion problem;
- [ ] use threads;
- [ ] better management of moves (especially with _big_ moves);  
- [ ] implements DAG version of coordinates' container finding;  
- [ ] do benchmarks between DAG and current algo;  
- [ ] implements other primitives for improve accessibility and usability;  
- [ ] vertex deleting;  
- [ ] upload packaged lib on pypi;


## MAYBE-A-DAY DO LIST
- support the third dimension;  


## DON'T DO LIST



## INDICATIONS FOR SWIG USING
SWIG is used for linking python and C/C++.
Triangulation is performed with C/C++ module _libdelaunator_, and Python have just to interface with it.

Makefile automatized each step of compilation, and filename modifications.

Basically:
- swig read the libdelaunator.i, a file interface that specify wich interface Python will have;
- swig generate C wrapper files (libdelaunator\_wrap.{c,h}) and a Python module libdelaunator.py;
- gcc compile all, including wrapper files, in shared mode for have a finally libdelaunator.so;
- libdelaunator.py is used by any Python code for interfacing with C module libdelaunator;

NB: the final library libdelaunator.so must have a name that begin by an '\_'.   
See second link after for comprehension.

### Some links
http://swig.org  
http://swig.org/tutorial.html  
http://stackoverflow.com/questions/10121957/swig-to-python-import-errordynamic-module-does-not-define-init-function  

