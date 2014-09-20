#ifndef TRIANGULED_OBJECT_H_INCLUDED
#define TRIANGULED_OBJECT_H_INCLUDED



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
class Vertex;




/**
 * TRIANGULED OBJECT CLASS
 */
class TrianguledObject {
	public:
	// CONSTRUCTOR
                TrianguledObject(Vertex* = NULL);
                ~TrianguledObject();
	// PUBLIC METHODS
	// ACCESSORS
                // getter
                Vertex* getVertex() const;
                std::list<TrianguledObject> neighborsAt(float) const;
                // setters
                void setVertex(Vertex*);
	private:
	// ATTRIBUTES
                Vertex* vertex;
	// PRIVATE METHODS
};


#endif
