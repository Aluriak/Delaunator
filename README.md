# Delaunator
2D Delaunay Triangulation in C++ with Python wrapper.  
(or wathever you want, thanks to SWIG)   
  
__LICENSE:__ _GPL-2.0_; for use, help and share. cf LICENSE.txt file.  
__Portability:__ code and tests on Debian Stable, with c++11, gcc 4.7.2, make 3.81, python 3.2 and SWIG 2.0; i don't know for others platforms, but except for windows, i think portability is totally possible, maybe already done. 


  

## PROGRADE DIRECTION
- [x] do triangulation; 
- [x] print it by Python script and pygame; 
- [x] do delaunay triangulation; 
- [x] manage movement;  
- [x] do a victory cry;  
- [x] upload packaged lib on pypi;  
- [ ] manage big/precise moves;  
- [ ] use threads;  
- [ ] find a better way than TTL limit for fix infinite recursion problem;  
- [ ] implements DAG version of coordinates' container finding;  
- [ ] do benchmarks between DAG and current vertex finder;  
- [ ] vertex deleting;  


## MAYBE-A-DAY DO LIST
- support the third dimension;  
- support constraints;
- built-in collision API;
- do a technical tutorial on quad-edge and other structure/algo used by this module;  
- do a technical tutorial on methodology for creat a package like this;  




## EXAMPLE OF USE
See github repo, pydelaunator repertory, for built-in example.

    from delaunator import Delaunator

    dt = Delaunator(600, 600)
    vtx = dt.addVertexAt(342, 23)

    dt.moveVertex(vtx, .5, -31)
    
    # print all neighbour vertices
    print([_ for _ in dt.getNeighbors(vtx)]])

API will be improved with time and ideas.





## INSTALLATION
A simple
    
    pip install delaunator  

must be enough.  
Please note that SWIG is needed. (see links below; SWIG is in official repositories in most of cases)  

Some problems can appear : 
- on my debian stable, SWIG is named *swig2.0*, but setuptools use *swig*. You can can correct that with command *python3 setup.py setopt --command=build_ext --option=swig --set-value=swig2.0*, or something like that.  
- be sur to have only one version of setuptools, sometimes there are problems when two different versions are installed.  

Contact maintainer by mail for help if necessary.





### Some links
https://github.com/Aluriak/Delaunator

http://swig.org    
http://swig.org/tutorial.html  

http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html

http://stackoverflow.com/questions/10121957/swig-to-python-import-errordynamic-module-does-not-define-init-function  

http://sametmax.com/creer-un-setup-py-et-mettre-sa-bibliotheque-python-en-ligne-sur-pypi/  
https://docs.python.org/3.2/distutils/setupscript.html#extension-source-files  
https://pythonhosted.org/setuptools/setuptools.html  

