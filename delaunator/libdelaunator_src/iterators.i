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






/*************************/
/* IteratorOnVertices */
/*************************/
%exception IteratorOnVertices::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnVertices {
        Vertex* __getitem__(size_t i) {
                if (i >= $self->getVertices()->size()-4) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getVertices())[i+4];
        }
}




/*************************/
/* IteratorOnAllVertices */
/*************************/
%exception IteratorOnAllVertices::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnAllVertices {
        Vertex* __getitem__(size_t i) {
                if (i >= $self->getVertices()->size()-4) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getVertices())[i+4];
        }
}






/*************************************/
/* IteratorVertexToNeighbourVertices */
/*************************************/
%exception IteratorVertexToNeighbourVertices::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; /* clear flag for next time */
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorVertexToNeighbourVertices {
        Vertex* __getitem__(size_t i) {
                if ($self->next() == NULL) {
                        Iterator_err = 1;
                        return NULL;
                }
                return $self->getItem();
        }
}





/*************************************/
/* IteratorVertexToNeighbourEdges */
/*************************************/
%exception IteratorVertexToNeighbourEdges::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; /* clear flag for next time */
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorVertexToNeighbourEdges {
        Edge* __getitem__(size_t i) {
                if ($self->next() == NULL) {
                        Iterator_err = 1;
                        return NULL;
                }
                return $self->getItem();
        }
}





/**********************/
/* IteratorOnFaces */
/**********************/
%exception IteratorOnFaces::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnFaces {
        Face* __getitem__(size_t i) {
                if (i >= $self->getFaces()->size()) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getFaces())[i];
        }
}





/**********************/
/* IteratorOnAllFaces */
/**********************/
%exception IteratorOnAllFaces::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnAllFaces {
        Face* __getitem__(size_t i) {
                if (i >= $self->getFaces()->size()) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getFaces())[i];
        }
}






/********************************/
/* IteratorFaceToNeighbourFaces */
/********************************/
%exception IteratorFaceToNeighbourFaces::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; /* clear flag for next time */
                /* You could also check the value in $result, but it's a PyObject here */
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorFaceToNeighbourFaces {
        Face* __getitem__(size_t i) {
                if (i >= 3) {
                        Iterator_err = 1;
                        return NULL;
                }
                return $self->next();
        }
}





/**********************/
/* IteratorOnEdges    */
/**********************/
%exception IteratorOnEdges::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnEdges {
        Edge* __getitem__(size_t i) {
                if (i >= $self->getEdges()->size()) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getEdges())[i];
        }
}





/**********************/
/* IteratorOnAllEdges */
/**********************/
%exception IteratorOnAllEdges::__getitem__ {
        assert(!Iterator_err);
        $action
        if (Iterator_err) {
                Iterator_err = 0; 
                SWIG_exception(SWIG_IndexError, "Index out of bounds");
        }
}

%extend IteratorOnAllEdges {
        Edge* __getitem__(size_t i) {
                if (i >= $self->getEdges()->size()) {
                        Iterator_err = 1;
                        return NULL;
                }
                return (*$self->getEdges())[i];
        }
}





