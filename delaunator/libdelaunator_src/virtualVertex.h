#ifndef VIRTUAL_VERTEX_H_INCLUDED
#define VIRTUAL_VERTEX_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "vertex.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Coordinates;
class Vertex;




/**
 * VIRTUAL VERTEX CLASS
 */
class VirtualVertex {
	public:
	// CONSTRUCTOR
                VirtualVertex(Vertex* = NULL);
                ~VirtualVertex();
	// PUBLIC METHODS
        // PREDICATS
                bool confoundedWithAnotherObject() const;
	// ACCESSORS
                // getter
                Vertex* vertex() const;
                unsigned int id() const { return this->_id; }
                Coordinates coordinates() const;
                // setters
                void setVertex(Vertex*);
        // GRAPH EXPLORATION
                std::list<VirtualVertex*> directNeighbors() const;
                std::list<VirtualVertex*> neighborsAt(float) const;
                std::list<VirtualVertex*> nearerNeighbors(unsigned int) const;
	private:
	// ATTRIBUTES
                Vertex* ref_vertex;
                unsigned int _id;
                static unsigned int last_id;
	// PRIVATE METHODS
};


#endif
