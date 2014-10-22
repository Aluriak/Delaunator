%module libdelaunator
%{
/* Includes the header in the wrapper code */
#include "delaunator.h"
%}

%include "std_string.i"
%include "std_list.i"
%include "std_vector.i"
%include "exception.i"
%include "typemaps.i"

namespace std {
        %template(VirtualVertexList) list<VirtualVertex*>;
}

%include "utils.h"
%include "commons.h"

%include "vertex.h"
%extend Coordinates {
        /* for have x and y as property in pythons modules */
        %pythoncode %{
                __swig_getmethods__["x"] = x
                __swig_setmethods__["x"] = setX
                __swig_getmethods__["y"] = y
                __swig_setmethods__["y"] = setY
                if _newclass: 
                        x = property(x, x)
                        y = property(y, y)
        %}
};

%include "edge.h"
%include "face.h"
%include "iterators.i"
%include "virtualVertex.h"
%include "triangulation.h"
%include "delaunator.h"

/* Create TrianguledObject interface class */
#if SWIGPYTHON
%include "python_trianguledObject.i"
#endif

