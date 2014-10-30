#ifndef DELAUNATOR_TRIANGULATION_H_INCLUDED
#define DELAUNATOR_TRIANGULATION_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "geometry.h"
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
/**
 * Finder mode for when triangulation looking for the Face that contain Coordinates 
 *   (notabily for Vertex adding).  
 * Random: random Edge in all existing in Triangulation instance
 * First: the first added Edge
 * Middle: the Edge in the middle of the Edge list
 * Last: the last added Edge (default value)
 * @note Last mode is certainly the better, especially in case where added Vertex are close to previous one
 */
enum VertexFinderMode {
        VERTEX_FINDER_MODE_RANDOM,
        VERTEX_FINDER_MODE_FIRST,
        VERTEX_FINDER_MODE_MIDDLE,
        VERTEX_FINDER_MODE_LAST
};



/*
 * For some explanations on quad-edge implementation :
 * http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html
*/





/**
 * Delaunay Triangulation class definition.  
 * Main object manipulated by user/wrapper.
 * Provide iterators, access to Vertices, Faces, Edges,...  
 * Have all methods for add, delete and move Vertices.
 */
class Triangulation {
	public:
        // INTERNAL CLASS TYPES
                // define type for pointer to function that looking for initial Edge
                typedef Edge* (Triangulation::*finderInitialEdge_mode)() const; 

	// CONSTRUCTOR
		Triangulation(const float, const float, 
                           const float, const float, const VertexFinderMode = VERTEX_FINDER_MODE_LAST);
		~Triangulation();
	// PUBLIC METHODS
                Vertex* addVertexAt(Coordinates, Edge* = NULL);
                Vertex* addVertexAt(float x, float y, Edge* e = NULL) 
                        { return this->addVertexAt(Coordinates(x, y), e); }
                Vertex* vertexAt(float, float, float=EPSILON) const;
                Vertex* vertexAt(Coordinates c, float p=EPSILON) const { return this->vertexAt(c.x(), c.y(), p); }
                Vertex* moveVertex(Vertex* v, Coordinates c) { return this->moveVertex(v, c.x(), c.y()); }
                Vertex* moveVertex(Vertex* v, float x, float y);
                Vertex* moveVertexTo(Vertex* v, Coordinates c);
                void delVertex(Vertex* v);
                void mergeVertex(Vertex* v, Vertex* v_destroyed);
#ifdef DEBUG // some tests with assertions
                void DEBUG_tests() const;
#endif
	// ACCESSORS
                unsigned int getIndexOf(Vertex*) const;
                std::vector<Edge*> getEdges()  const { return this->edges; }
                unsigned int getVerticeCount() const { return this->vertices.size(); }
                float getXmin() const { return this->xmin; }
                float getXmax() const { return this->xmax; }
                float getYmin() const { return this->ymin; }
                float getYmax() const { return this->ymax; }
                float epsilon() const { return EPSILON; }
                VertexFinderMode getFinderMode() const;
                void setFinderMode(VertexFinderMode); 

        // PREDICATS
                bool haveVertex(Vertex*) const;
                bool collideAt(Coordinates) const;
#ifdef DEBUG
                bool opt_isdebug()       const { return true; }
#else
                bool opt_isdebug()       const { return false; }
#endif
#ifdef FOLLOW_SEARCH
                bool opt_follow_search() const { return true; }
#else
                bool opt_follow_search() const { return false; }
#endif


        // ITERATORS
                IteratorVertexToNeighbourVertices getNeighbors(Vertex*);
                // edges necessary for user
                IteratorOnEdges iterEdges()             { return IteratorOnEdges(&this->edges); }
                IteratorOnEdges_read iterEdges_read() const
                                                        { return IteratorOnEdges_read(&this->edges); }
                // all edges, including the externals ones
                IteratorOnAllEdges iterAllEdges()       { return IteratorOnAllEdges(&this->edges); }
                IteratorOnAllEdges_read iterAllEdges_read() const
                                                        { return IteratorOnAllEdges_read(&this->edges); }
                // faces that are visible to user
                IteratorOnFaces iterFaces()             { return IteratorOnFaces(&this->faces); }
                IteratorOnFaces_read iterFaces_read() const
                                                        { return IteratorOnFaces_read(&this->faces); }
                // all faces, including the unvisible ones
                IteratorOnAllFaces iterAllFaces()       { return IteratorOnAllFaces(&this->faces); }
                IteratorOnAllFaces_read iterAllFaces_read() const
                                                        { return IteratorOnAllFaces_read(&this->faces); }
                // vertices placed by user
                IteratorOnVertices iterVertices()       { return IteratorOnVertices(&this->vertices); }
                IteratorOnVertices_read iterVertices_read() const 
                                                        { return IteratorOnVertices_read(&this->vertices); }
                // all vertices, including the 4 used for create and maintain the mesh
                IteratorOnAllVertices iterAllVertices() { return IteratorOnAllVertices(&this->vertices); }
                IteratorOnAllVertices_read iterAllVertices_read() const 
                                                        { return IteratorOnAllVertices_read(&this->vertices); }




// PRIVATE
	private:
	// ATTRIBUTES
                float xmin, xmax, ymin, ymax;
                std::vector<Vertex*> vertices;
                std::vector<Edge*> edges;
                std::vector<Face*> faces;
                finderInitialEdge_mode finderInitialEdge = NULL; // pointer to func that looking for initial Edge
	// PRIVATE METHODS
                Face* findContainerOf(Coordinates, Edge* = NULL) const;
                // methods for choose initial Edges. The effectively used is pointed by finderInitialEdge.
                Edge* finderInitial_random() const { return this->edges[randN(this->edges.size())]; }
                Edge* finderInitial_middle() const { return this->edges[randN(this->edges.size()/2)]; }
                Edge* finderInitial_first () const { return this->edges[0]; }
                Edge* finderInitial_last  () const { return this->edges[this->edges.size()-1]; }
#ifdef DEBUG
                bool applyDelaunayCondition(Face*, unsigned int ttl = 0);
#else
                bool applyDelaunayCondition(Face*);
#endif
                void operateFlip(Edge*);
                // Methods for manipulate lists of components
                inline void removeVertexFromVertices(Vertex* v) {
                        for(std::vector<Vertex*>::iterator it = this->vertices.begin(); 
                                        it != this->vertices.end(); it++) {
                                if((*it) == v) {
                                        this->vertices.erase(it);
                                        delete v;
                                        it = this->vertices.end()-1;
                                }
                        }
                }
                inline void removeEdgeFromEdges(Edge* e) {
                        for(std::vector<Edge*>::iterator it = this->edges.begin(); 
                                        it != this->edges.end(); it++) {
                                if((*it) == e) {
                                        this->edges.erase(it);
                                        delete e;
                                        it = this->edges.end()-1;
                                }
                        }
                }
                inline void removeFaceFromFaces(Face* f) {
                        for(std::vector<Face*>::iterator it = this->faces.begin(); 
                                        it != this->faces.end(); it++) {
                                if((*it) == f) {
                                        this->faces.erase(it);
                                        delete f;
                                        it = this->faces.end()-1;
                                }
                        }
                }
                /*
                 * Replace given vertex coords by given values.
                 */
                inline void moveVertex_pure(Vertex* v, Coordinates new_p) {
                // Move the vertex
                        v->setX(new_p.x());
                        v->setY(new_p.y());
                // Apply Delaunay Condition
                        std::vector<Face*> nei_faces;
                        Edge* edge = v->getEdge();
                        do {
                                nei_faces.push_back(edge->leftFace());
                                edge = edge->rotLeftEdge();
                        } while(edge != v->getEdge());

                        for(Face* f : nei_faces) {
#ifdef DEBUG
                                assert(f != NULL);
#endif
                                this->applyDelaunayCondition(f);
                        }
                }
};



#endif
