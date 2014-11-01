#ifndef DELAUNATOR_EDGE_ITERATORS_H_INCLUDED
#define DELAUNATOR_EDGE_ITERATORS_H_INCLUDED



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
 * ITERATORS ON ALL EDGES
 ******************************************************************/
class IteratorOnEdges {
	public:
	// CONSTRUCTOR
		IteratorOnEdges(std::vector<Edge*>* v) : edges(v) {
                        this->it = this->edges->begin();
                        // ignore the edges that are not necessary for linked user Vertex
                        while((*this->it)->isExternal()) this->it++;
                }
	// PUBLIC METHODS
                IteratorOnEdges& next()      { return (*this)++; }
	// OPERATORS
                bool operator!=(const IteratorOnEdges& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Edge*>::iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnEdges& operator++(int i) { // post increment
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
class IteratorOnEdges_read {
	public:
	// CONSTRUCTOR
		IteratorOnEdges_read(const std::vector<Edge*>* const v) : edges(v) {
                        this->it = this->edges->begin();
                        // ignore the edges that are not necessary for linked user Vertex
                        while((*this->it)->isExternal()) this->it++;
                }
	// PUBLIC METHODS
                IteratorOnEdges_read& next()      { return (*this)++; }
	// OPERATORS
                bool operator!=(const IteratorOnEdges_read& othr) const { 
                        return this->it != othr.it;
                }
                bool operator!=(const std::vector<Edge*>::const_iterator& ot) const { 
                        return this->it != ot;
                }
                IteratorOnEdges_read& operator++(int i) { // post increment
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
