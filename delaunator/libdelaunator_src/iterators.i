%module iterator
%{
/* Includes the header in the wrapper code */
#include "iterators.h"
#include "vertex_iterators.h"
#include "face_iterators.h"
#include "edge_iterators.h"
static int Iterator_err = 0;
%}

%include "iterators.h"
%include "vertex_iterators.h"
%include "face_iterators.h"
%include "edge_iterators.h"

