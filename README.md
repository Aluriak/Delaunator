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
- [x] manage confunded Vertex;  
- [x] manage internal association between Vertex and associated user objects;
- [x] write API documentation;  
- [x] write spec file;  
- [ ] manage groups of vertices that moves together; [Next goal]
- [ ] support constraints; [Next goal]
- [ ] built in collision management; [Next goal]
- [ ] access in constant complexity to nearer neighbour; [PoC on a branch]
- [ ] optimize Vertex, Edge and Face containing; [One day]
- [ ] use threads; [One day]  
- [ ] implement iterator on the N nearest neighbors of a vertex;  [In progress] 
- [ ] implement iterator on the N nearest neighbors of a vertex that respect a predicat; [In progress]  
- [ ] implement iterator on the neighbors that are away from a max distance; [In progress]  
- [ ] permit to user to choose preprocessor options by config files or other useful way;  [One day] 
- [ ] improve API documentation; [One day]
- [ ] improve spec file; [One day]
- [ ] implement MultiTrianguledObject, that can be present in multiple Delaunator instances; [One day]
- [ ] find a way to retract Face class (because its useless for user); [One day]


## MAYBE-A-DAY DO LIST
- support the third dimension;  
- implements pure python version of this module & do benchmarks between it and current implementation;
- be brave and keep the pure python version if its quicker;
- do a technical tutorial on quad-edge and other structure/algo used by this module;  
- do a technical tutorial on methodology for creat a package like this;  
- build documentation of source, presented in Python style;  




## EXAMPLE OF USE
See github repo, pydelaunator repertory, for built-in example.


    from random import randint
    from delaunator import Delaunator, TrianguledObject

    # useful class
    class Student(TrianguledObject): 
        def __init__(self, name):
            super().__init__()
            self.name = str(name)

        def presentation(self, othr=None):
            if othr is not None:
                d = str(othr.distanceTo(self)) 
                return self.name + " " + "("+d+")"
            else:
                return self.name

    # bounds
    xmin, xmax, ymin, ymax = 0, 600, 0, 600

    # creat delaunator and data
    dt = Delaunator(xmin, xmax, ymin, ymax)
    michel = Student('michel')
    dt.addTrianguledObject(michel, (342, 123))

    for i in range(3):
        dt.addTrianguledObject(Student('totoro'), (randint(xmin, xmax), randint(ymin, ymax)))

    # movement
    dt.movTrianguledObject(michel, (randint(xmin, xmax) / 2, randint(ymin, ymax) / 2))

    # print name of all neighbors that are to a distance at most 100
    print("My neighbors are " + ", ".join([_.presentation(michel) for _ in michel.neighborsAt(300)]))
    print("The others are   " + ", ".join([_.presentation(michel) for _ in michel.neighborsAt(dt.distanceMax(), 300)]))
    print("All are          " + ", ".join([_.presentation(michel) for _ in michel.neighborsAt(dt.distanceMax())]))

    # frees
    dt.delTrianguledObject(michel)

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

Some documents in doc/ repertory are not generated by doxygen: its keyboard-writed documents, notabily specification document.


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

