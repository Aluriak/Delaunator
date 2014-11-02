#ifndef DELAUNATOR_FACE_ITERATORS_H_INCLUDED
#define DELAUNATOR_FACE_ITERATORS_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "vertex.h"
#include "face.h"
#include "edge.h"
#include "iterators.h"


/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
// NB: iterators defined next() function for portability to Python's wraps, 
//      or other language where ++ unary operator don't exist.
//      So, next() and operator++ are equivalent.




/******************************************************************
 * ITERATORS ON FACES 
 ******************************************************************/
class IteratorOnFaces {
	public:
	// CONSTRUCTOR
		IteratorOnFaces(std::list<Face*>* v) : faces(v) {
                        this->it = this->faces->begin();
                        // ignore the faces that are not visible
                        while(!(*this->it)->isVisible()) this->it++; 
                }
	// PUBLIC METHODS
                IteratorOnFaces& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnFaces& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::list<Face*>::iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnFaces& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::list<Face*>::iterator begin() const { return this->faces->begin(); }
                std::list<Face*>::iterator end()   const { return this->faces->end(); }
                std::list<Face*>* getFaces()       const { return this->faces; }
        protected:
	// ATTRIBUTES
                std::list<Face*>* faces;
                std::list<Face*>::iterator it;
	// PRIVATE METHODS
};
class IteratorOnFaces_read {
	public:
	// CONSTRUCTOR
		IteratorOnFaces_read(const std::list<Face*>* const v) : faces(v) {
                        this->it = this->faces->begin();
                        // ignore the faces that are not visible
                        while(!(*this->it)->isVisible()) this->it++; 
                }
	// PUBLIC METHODS
                IteratorOnFaces_read& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnFaces_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::list<Face*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnFaces_read& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::list<Face*>::const_iterator begin() const { return this->faces->begin(); }
                std::list<Face*>::const_iterator end()   const { return this->faces->end(); }
                const std::list<Face*>* getFaces() const { return this->faces; }
        protected:
	// ATTRIBUTES
                const std::list<Face*>* const faces;
                std::list<Face*>::const_iterator it;
	// PRIVATE METHODS
};





/******************************************************************
 * ITERATORS ON ALL FACES 
 ******************************************************************/
class IteratorOnAllFaces {
	public:
	// CONSTRUCTOR
		IteratorOnAllFaces(std::list<Face*>* v) : faces(v) {
                        this->it = this->faces->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllFaces& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnAllFaces& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::list<Face*>::iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllFaces& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::list<Face*>::iterator begin() const { return this->faces->begin(); }
                std::list<Face*>::iterator end()   const { return this->faces->end(); }
                std::list<Face*>* getFaces()       const { return this->faces; }
        protected:
	// ATTRIBUTES
                std::list<Face*>* faces;
                std::list<Face*>::iterator it;
	// PRIVATE METHODS
};
class IteratorOnAllFaces_read {
	public:
	// CONSTRUCTOR
		IteratorOnAllFaces_read(const std::list<Face*>* const v) : faces(v) {
                        this->it = this->faces->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllFaces_read& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnAllFaces_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::list<Face*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllFaces_read& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::list<Face*>::const_iterator begin() const { return this->faces->begin(); }
                std::list<Face*>::const_iterator end()   const { return this->faces->end(); }
                const std::list<Face*>* getFaces() const { return this->faces; }
        protected:
	// ATTRIBUTES
                const std::list<Face*>* const faces;
                std::list<Face*>::const_iterator it;
	// PRIVATE METHODS
};








/******************************************************************
 * ITERATORS FACE TO VERTICES
 ******************************************************************/
class IteratorFaceToVertices {
        // iterate on the three Vertices of a Face.
	public:
	// CONSTRUCTOR
		IteratorFaceToVertices(const Face* const f) : edge(f->getEdge()) {
                        this->ref_edge = this->edge;
                }
	// PUBLIC METHODS
                IteratorFaceToVertices& next() { 
                        this->edge = this->edge->nextLeftEdge();
                        if(this->edge == this->ref_edge)
                                this->edge = NULL;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorFaceToVertices& othr) const { 
                        return this->edge != othr.edge;
                }
                bool operator!=(const Edge* ot) const { 
                        return this->edge != ot;
                }
                IteratorFaceToVertices& operator++(int) { return this->next(); }
                Vertex* operator*() { // pointer dereferences
                        return this->edge->originVertex();
                }
	// ACCESSORS
                Edge* begin() const { return this->ref_edge; }
                Edge* end()   const { return NULL; }
        protected:
	// ATTRIBUTES
                Edge* edge;
                Edge* ref_edge;
	// PRIVATE METHODS
};






/******************************************************************
 * ITERATORS FACE TO EDGES
 ******************************************************************/
class IteratorFaceToEdges {
        // iterate on the three Edges of a Face.
	public:
	// CONSTRUCTOR
		IteratorFaceToEdges(const Face* const f) : edge(f->getEdge()) {
                        this->ref_edge = this->edge;
                }
	// PUBLIC METHODS
                IteratorFaceToEdges& next() { 
                        this->edge = this->edge->nextLeftEdge();
                        if(this->edge == this->ref_edge)
                                this->edge = NULL;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorFaceToEdges& othr) const { 
                        return this->edge != othr.edge;
                }
                bool operator!=(const Edge* ot) const { 
                        return this->edge != ot;
                }
                IteratorFaceToEdges& operator++(int) { return this->next(); }
                Edge* operator*() { // pointer dereferences
                        return this->edge;
                }
	// ACCESSORS
                Edge* begin() const { return this->ref_edge; }
                Edge* end()   const { return NULL; }
        protected:
	// ATTRIBUTES
                Edge* edge;
                Edge* ref_edge;
	// PRIVATE METHODS
};






/******************************************************************
 * ITERATORS FACE TO NEIGHBOUR FACES
 ******************************************************************/
class IteratorFaceToNeighbourFaces {
	public:
	// CONSTRUCTOR
		IteratorFaceToNeighbourFaces(Face* v) : edge(v->getEdge()) {}
	// PUBLIC METHODS
                Face* next() { 
                        this->edge = this->edge->nextLeftEdge();
                        return this->edge->rightFace();
                }
	// ACCESSORS
	protected:
	// ATTRIBUTES
                Edge* edge;
	// PRIVATE METHODS
};






#endif
