%module libdelaunator
%{
/* Includes the header in the wrapper code */
#include "delaunator.h"
static int IteratorOnAllEdges_myErr = 0;
%}

%include "std_string.i"
%include "std_vector.i"
%include "exception.i"
%exception std::vector::getitem {
        try {
                $action
        } catch (std::out_of_range& e) {
                SWIG_exception(SWIG_IndexError,const_cast<char*>(e.what()));
        }
}
/* Parse the header file to generate wrappers */

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

%include "delaunator.h"
