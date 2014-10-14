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
- [x] doxygen integration;
- [x] find a better way than TTL limit for fix infinite recursion problem;  (find: better management of computing precision)  
- [x] vertex deleting;  
- [x] manage big/precise moves;  
- [x] permit to user to choose finder options at Delaunator instanciation;  
- [ ] manage confunded Vertex;  
- [ ] manage internal association between Vertex and associated user objects;
- [ ] access in constant complexity to nearer neighbour;
- [ ] manage groups of vertices that moves together;
- [ ] optimize Vertex, Edge and Face containing;
- [ ] use threads;  
- [ ] implement iterator on the N nearest neighbors of a vertex;  
- [ ] implement iterator on the N nearest neighbors of a vertex that respect a predicat;  
- [ ] permit to user to choose preprocessor options by config files or other useful way;  
- [ ] write API documentation;
- [ ] allow to TrianguledObject to be present in multiple Delaunator instances;


## MAYBE-A-DAY DO LIST
- support constraints;
- built-in collision API;
- support the third dimension;  
- implements DAG version of coordinates' container finding & do benchmarks between it and current finder;  
- do a technical tutorial on quad-edge and other structure/algo used by this module;  
- do a technical tutorial on methodology for creat a package like this;  
- build documentation of source, presented in Python style;  




## EXAMPLE OF USE
See github repo, pydelaunator repertory, for built-in example.

    from random import randint
    from delaunator import Delaunator, TrianguledObject

    dt = Delaunator(0, 500, 0, 500)
    class People(object):
        def __init(self, vertex, name):
            self.name = name
            self.vertex = vertex

        def __str__(self):
            return self.name


    objs = {}
    michel = dt.addObject(randint(0, 600), randint(0, 600))
    objs[michel] = People(michel, 'michel')

    for i in range(100):
        newobj = dt.addObject(randint(0,600), randint(0,600))
        objs[newobj] = People(newobj, 'totoro')

    print("My neighbors are " + ", ".join([str(objs[_]) for _ in michel.neighborsAt(100)]))



    # useful class
    class MyObject(TrianguledObject): 
        def __init__(self, name):
            self.name = str(name)
        def __str__(self):
            return self.name

    dt = Delaunator(0, 600, 0, 600)
    michel = MyObject('michel')
    dt.addObjectAt(342, 123, michel)

    for i in range(100):
        dt.addObjectAt(randint(0,600), randint(0,600), MyObject('totoro'))

    dt.moveObject(michel, 23.5, -31)
    
    # print name of all neighbors that are to a distance at most 100
    print("My neighbors are " + ", ".join([str(_) for _ in michel.neighborsAt(100)]))
    
    dt.delObject(michel)
    dt.delAllObjects()

API will be improved with time and ideas.





## INSTALLATION
A simple
    
    pip install delaunator  

must be enough.  
Please note that SWIG is needed. (see links below; SWIG is in official repositories in most of cases)  

Some problems can appear : 
- on my debian stable, SWIG is named *swig2.0*, but setuptools call *swig*. You can correct that with command *python3 setup.py setopt --command=build_ext --option=swig --set-value=swig2.0*, or something like that.  
- be sur to have only one version of setuptools, sometimes there are problems when two different versions are installed.  

Contact maintainer by mail for help if necessary.

### Options in installations
You can edit setup.py file, and change lines about 38, where DEBUG macro is defined. When DEBUG is defined, 
lots of verifications are done (by assert calls), so delete DEBUG will increase performances but remove detection of corruptions;

Feel free to modify theses. (notabily delete DEBUG, if you want use delaunator in the faster way)  




## DOCUMENTATION GENERATION
Generate the documentation require Doxygen (or something that works like it) and can be performed with a simple

    make gendoc

or 

    doxygen Doxyfile

HTML and Latex documentations are generated in doc repertory.  
An online documentation can be found at pythonhosted.org. (see links below)  



## USE DOCUMENTATION
The documentation is generated with Doxygen by parsing C++ source. 
However, as SWIG creat Python wrapper by parsing same source, documentation is usable for Python codes.  

Maybe later, i will found a solution for generate pure Python documentation.  



### Some links
https://github.com/Aluriak/Delaunator  
https://pypi.python.org/pypi/delaunator  
https://pythonhosted.org/delaunator/  

http://swig.org    
http://swig.org/tutorial.html  

http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html

http://stackoverflow.com/questions/10121957/swig-to-python-import-errordynamic-module-does-not-define-init-function  

http://sametmax.com/creer-un-setup-py-et-mettre-sa-bibliotheque-python-en-ligne-sur-pypi/  
https://docs.python.org/3.2/distutils/setupscript.html#extension-source-files  
https://pythonhosted.org/setuptools/setuptools.html  

