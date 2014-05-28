#ifndef DELAUNATOR_ITERATOR_H_INCLUDED
#define DELAUNATOR_ITERATOR_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "vertex.h"
#include "face.h"
#include "edge.h"


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
 * ITERATORS ON ALL VERTICES
 ******************************************************************/
class IteratorOnAllVertices {
	public:
	// CONSTRUCTOR
		IteratorOnAllVertices(std::vector<Vertex*>* v) : vertices(v) {
                        this->it = this->vertices->begin()+4;
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
                        this->next(); this->next(); this->next(); this->next();
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




/******************************************************************
 * ITERATORS ON FACE 
 ******************************************************************/
class IteratorOnAllFaces {
	public:
	// CONSTRUCTOR
		IteratorOnAllFaces(std::vector<Face*>* v) : faces(v) {
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
                bool operator!=(const std::vector<Face*>::iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllFaces& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Face*>::iterator begin() const { return this->faces->begin(); }
                std::vector<Face*>::iterator end()   const { return this->faces->end(); }
                std::vector<Face*>* getFaces()       const { return this->faces; }
        protected:
	// ATTRIBUTES
                std::vector<Face*>* faces;
                std::vector<Face*>::iterator it;
	// PRIVATE METHODS
};
class IteratorOnAllFaces_read {
	public:
	// CONSTRUCTOR
		IteratorOnAllFaces_read(const std::vector<Face*>* const v) : faces(v) {
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
                bool operator!=(const std::vector<Face*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllFaces_read& operator++(int) { return this->next(); }
                Face* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Face*>::const_iterator begin() const { return this->faces->begin(); }
                std::vector<Face*>::const_iterator end()   const { return this->faces->end(); }
                const std::vector<Face*>* getFaces() const { return this->faces; }
        protected:
	// ATTRIBUTES
                const std::vector<Face*>* const faces;
                std::vector<Face*>::const_iterator it;
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




/******************************************************************
 * ITERATORS ON ALL EDGES
 ******************************************************************/
class IteratorOnAllEdges {
	public:
	// CONSTRUCTOR
		IteratorOnAllEdges(std::vector<Edge*>* v) : edges(v) {
                        this->it = this->edges->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllEdges& next()      { return (*this)++; }
	// OPERATORS
                bool operator!=(const IteratorOnAllEdges& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Edge*>::iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllEdges& operator++(int i) { // post increment
                        this->it++;
                        return *this;
                }
                Edge* operator*() { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Edge*>::iterator begin()    const { return this->edges->begin(); }
                std::vector<Edge*>::iterator end()      const { return this->edges->end(); }
                std::vector<Edge*>* getEdges()          const { return this->edges; }
        protected:
	// ATTRIBUTES
                std::vector<Edge*>* edges;
                std::vector<Edge*>::iterator it;
	// PRIVATE METHODS
};
class IteratorOnAllEdges_read {
	public:
	// CONSTRUCTOR
		IteratorOnAllEdges_read(const std::vector<Edge*>* const v) : edges(v) {
                        this->it = this->edges->begin();
                }
	// PUBLIC METHODS
                IteratorOnAllEdges_read& next()      { return (*this)++; }
	// OPERATORS
                bool operator!=(const IteratorOnAllEdges_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Edge*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnAllEdges_read& operator++(int i) { // post increment
                        this->it++;
                        return *this;
                }
                Edge* operator*() const { //pointer dereferences
                        return *(this->it);
                }
	// ACCESSORS
                std::vector<Edge*>::const_iterator begin()    const { return this->edges->begin(); }
                std::vector<Edge*>::const_iterator end()      const { return this->edges->end(); }
                const std::vector<Edge*>* getEdges()          const { return this->edges; }
        protected:
	// ATTRIBUTES
                const std::vector<Edge*>* const edges;
                std::vector<Edge*>::const_iterator it;
	// PRIVATE METHODS
};


#endif
