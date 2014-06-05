#ifndef DELAUNATOR_TRIANGULATION_H_INCLUDED
#define DELAUNATOR_TRIANGULATION_H_INCLUDED



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
#define DELAUNAY_CONDITION 1



/*
 * PREDECLARATIONS
 */



/*
 * For some explanations on quad-edge implementation :
 * http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html
*/




/*
 * DELAUNAY CLASS DEFINITION
 */
class Delaunator {
	public:
	// CONSTRUCTOR
		Delaunator(const float, const float, const float, const float);
		~Delaunator();
	// PUBLIC METHODS
                Vertex* addVertexAt(Coordinates);
                Vertex* addVertexAt(float x, float y) { return this->addVertexAt(Coordinates(x, y)); }
                Vertex* vertexAt(float, float, float) const;
                Vertex* vertexAt(Coordinates c, float p) const { return this->vertexAt(c.x(), c.y(), p); }
                void moveVertex(Vertex* v, float vec_x, float vec_y);
                void moveVertex(Vertex* v, Coordinates c) { this->moveVertex(v, c.x(), c.y()); }
                bool collideAt(Coordinates) const;
#if DEBUG // some tests with assertions
                void DEBUG_tests() const;
#endif
	// ACCESSORS
                std::vector<Edge*> getEdges() const { return this->edges; }
                float getXmin() const { return this->xmin; }
                float getXmax() const { return this->xmax; }
                float getYmin() const { return this->ymin; }
                float getYmax() const { return this->ymax; }
                unsigned int getVerticeCount() const { return this->vertices.size(); }
                float epsilon() const { return EPSILON; }
#ifdef FOLLOW_SEARCH
                bool opt_follow_search() const { return true; }
#else
                bool opt_follow_search() const { return false; }
#endif
        // ITERATORS
                IteratorVertexToNeighbourVertices getNeighbors(Vertex*);
                IteratorOnAllEdges allEdges()           { return IteratorOnAllEdges(&this->edges); }
                IteratorOnAllEdges_read allEdges_read() const
                                                        { return IteratorOnAllEdges_read(&this->edges); }
                IteratorOnAllFaces allFaces()           { return IteratorOnAllFaces(&this->faces); }
                IteratorOnAllFaces_read allFaces_read() const
                                                        { return IteratorOnAllFaces_read(&this->faces); }
                IteratorOnAllVertices allVertices()     { return IteratorOnAllVertices(&this->vertices); }
                IteratorOnAllVertices_read allVertices_read() const 
                                                        { return IteratorOnAllVertices_read(&this->vertices); }
	// ACCESSORS
	private:
	// ATTRIBUTES
                float xmin, xmax, ymin, ymax;
                std::vector<Vertex*> vertices;
                std::vector<Edge*> edges;
                std::vector<Face*> faces;
	// PRIVATE METHODS
                Face* findContainerOf(Coordinates) const;
                bool flipOn(Face*, unsigned int ttl = 0);
};



#endif
