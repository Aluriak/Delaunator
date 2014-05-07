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

%include "edge.h"

%include "face.h"

%include "iterators.i"

%include "delaunator.h"
