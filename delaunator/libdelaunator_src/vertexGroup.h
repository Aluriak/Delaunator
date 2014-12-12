#ifndef VERTEX_GROUP_H_INCLUDED
#define VERTEX_GROUP_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "virtualVertex.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Edge;





/**
 * VertexGroup are Vertex, according to Composite pattern.
 * Vertices of a VertexGroup moves together.
 * IMPLEMENTATION INCOMPLETE
 */
class VertexGroup : public Vertex {
        public:
        // CONSTRUCTOR
                VertexGroup();
                ~VertexGroup();
        // PUBLIC METHODS
                unsigned int neighbourCount() const;
                void take(VirtualVertex*, Vertex* = NULL);
                void giveVirtualVerticesTo(Vertex*);
                void forget(VirtualVertex*);
        // ACCESSORS
                unsigned int getObjectCount()   const;
                unsigned int getVertexCount()   const { return this->vertices.size(); }
                std::list<VirtualVertex*> getObjects(unsigned int = 0) const;
                VirtualVertex* getFirstObject()        const;
        // PREDICATS
                bool isNeighbourOf(Vertex*) const;
                bool isACorner() const;
                bool have(VirtualVertex*) const;
        private:
        // ATTRIBUTES
                std::list<Vertex*> vertices; // Vertex that composed this Group
};







#endif
