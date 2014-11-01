#ifndef DELAUNATOR_VERTEX_ITERATORS_H_INCLUDED
#define DELAUNATOR_VERTEX_ITERATORS_H_INCLUDED



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
 * ITERATORS ON VERTICES
 ******************************************************************/
class IteratorOnVertices {
	public:
	// CONSTRUCTOR
		IteratorOnVertices(std::vector<Vertex*>* v) : vertices(v) {
                        // ignore the vertices that are not asked by user
                        this->it = this->vertices->begin()+4;
                }
	// PUBLIC METHODS
                IteratorOnVertices& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnVertices& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Vertex*>::iterator& ot) const{ 
                        return this->it != ot;
                }
                IteratorOnVertices& operator++(int) { return this->next(); }
                Vertex* operator*() { // pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Vertex*>::iterator begin()  const { return this->vertices->begin(); }
                std::vector<Vertex*>::iterator end()    const { return this->vertices->end(); }
                std::vector<Vertex*>* getVertices()     const { return this->vertices; }
        protected:
	// ATTRIBUTES
                std::vector<Vertex*>* vertices;
                std::vector<Vertex*>::const_iterator it;
	// PRIVATE METHODS
};
class IteratorOnVertices_read {
	public:
	// CONSTRUCTOR
		IteratorOnVertices_read(const std::vector<Vertex*>* const v) : vertices(v) {
                        // ignore the vertices that are not asked by user
                        this->it = this->vertices->begin()+4;
                }
	// PUBLIC METHODS
                IteratorOnVertices_read& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnVertices_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Vertex*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnVertices_read& operator++(int) { return this->next(); }
                Vertex* operator*() { // pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Vertex*>::const_iterator begin()  const { return this->vertices->begin(); }
                std::vector<Vertex*>::const_iterator end()    const { return this->vertices->end(); }
                const std::vector<Vertex*>* const getVertices()     const { return this->vertices; }
        protected:
	// ATTRIBUTES
                const std::vector<Vertex*>* const vertices;
                std::vector<Vertex*>::const_iterator it;
	// PRIVATE METHODS
};








/******************************************************************
 * ITERATORS ON ALL VERTICES
 ******************************************************************/
class IteratorOnAllVertices {
	public:
	// CONSTRUCTOR
		IteratorOnAllVertices(std::vector<Vertex*>* v) : vertices(v) {
                        this->it = this->vertices->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllVertices& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnAllVertices& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Vertex*>::iterator& ot) const{ 
                        return this->it != ot;
                }
                IteratorOnAllVertices& operator++(int) { return this->next(); }
                Vertex* operator*() { // pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Vertex*>::iterator begin()  const { return this->vertices->begin(); }
                std::vector<Vertex*>::iterator end()    const { return this->vertices->end(); }
                std::vector<Vertex*>* getVertices()     const { return this->vertices; }
        protected:
	// ATTRIBUTES
                std::vector<Vertex*>* vertices;
                std::vector<Vertex*>::const_iterator it;
	// PRIVATE METHODS
};
class IteratorOnAllVertices_read {
	public:
	// CONSTRUCTOR
		IteratorOnAllVertices_read(const std::vector<Vertex*>* const v) : vertices(v) {
                        this->it = this->vertices->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllVertices_read& next() { 
                        this->it++;
                        return *this;
                }
	// OPERATORS
                bool operator!=(const IteratorOnAllVertices_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Vertex*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllVertices_read& operator++(int) { return this->next(); }
                Vertex* operator*() { // pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Vertex*>::const_iterator begin()  const { return this->vertices->begin(); }
                std::vector<Vertex*>::const_iterator end()    const { return this->vertices->end(); }
                const std::vector<Vertex*>* const getVertices()     const { return this->vertices; }
        protected:
	// ATTRIBUTES
                const std::vector<Vertex*>* const vertices;
                std::vector<Vertex*>::const_iterator it;
	// PRIVATE METHODS
};





/******************************************************************
 * ITERATORS VERTEX TO NEIGHBOUR VERTICES
 ******************************************************************/
class IteratorVertexToNeighbourVertices {
	public:
	// CONSTRUCTOR
		IteratorVertexToNeighbourVertices(Vertex* v) : it(v->getEdge()) {
                        this->ref = it;
                }
	// PUBLIC METHODS
	// OPERATORS
                IteratorVertexToNeighbourVertices& next() { 
                        this->it = this->it->rotLeftEdge();
                        if(this->it == this->ref) this->it = NULL;
                        return *this;
                }
                bool operator!=(const IteratorVertexToNeighbourVertices& othr) const { 
                        return this->it != othr.it;
                }
                bool operator==(const IteratorVertexToNeighbourVertices& othr) const { 
                        return this->it == othr.it;
                }
                bool operator!=(const Edge* ot) const { 
                        return this->it != ot;
                }
                bool operator==(const Edge* ot) const { 
                        return this->it == ot;
                }
                IteratorVertexToNeighbourVertices& operator++(int) { return this->next(); }
                Vertex* operator*() { //pointer dereferences
                        return this->getItem();
                }
	// ACCESSORS
                Edge* end()     const { return NULL; }
                Vertex* getItem() const { return this->it->destinVertex(); }
	protected:
	// ATTRIBUTES
                Edge *it, *ref;
	// PRIVATE METHODS
};






/******************************************************************
 * ITERATORS VERTEX TO NEIGHBOUR EDGES
 ******************************************************************/
class IteratorVertexToNeighbourEdges {
	public:
	// CONSTRUCTOR
		IteratorVertexToNeighbourEdges(Vertex* v) : it(v->getEdge()) {
                        this->ref = it;
                }
	// PUBLIC METHODS
	// OPERATORS
                IteratorVertexToNeighbourEdges& next() { 
                        this->it = this->it->rotLeftEdge();
                        if(this->it == this->ref) this->it = NULL;
                        return *this;
                }
                bool operator!=(const IteratorVertexToNeighbourEdges& othr) const { 
                        return this->it != othr.it;
                }
                bool operator==(const IteratorVertexToNeighbourEdges& othr) const { 
                        return this->it == othr.it;
                }
                bool operator!=(const Edge* ot) const { 
                        return this->it != ot;
                }
                bool operator==(const Edge* ot) const { 
                        return this->it == ot;
                }
                IteratorVertexToNeighbourEdges& operator++(int) { return this->next(); }
                Edge* operator*() { //pointer dereferences
                        return this->getItem();
                }
	// ACCESSORS
                Edge* end()     const { return NULL; }
                Edge* getItem() const { return this->it; }
	protected:
	// ATTRIBUTES
                Edge *it, *ref;
	// PRIVATE METHODS
};





#endif
