%module iterator
%{
/* Includes the header in the wrapper code */
#include "iterators.h"
#include "vertex_iterators.h"
#include "face_iterators.h"
#include "edge_iterators.h"
%}

%include "iterators.h"
%include "vertex_iterators.h"
%include "face_iterators.h"
%include "edge_iterators.h"

