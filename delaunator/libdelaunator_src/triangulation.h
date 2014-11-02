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
                Coordinates coordinateCorrection(Coordinates) const;
#ifdef DEBUG // some tests with assertions
                void DEBUG_tests() const;
#endif
	// ACCESSORS
                std::list<Edge*> getEdges()  const { return this->edges; }
                unsigned int getVerticeCount() const { return this->vertices.size(); }
                float getXmin() const { return this->xmin; }
                float getXmax() const { return this->xmax; }
                float getYmin() const { return this->ymin; }
                float getYmax() const { return this->ymax; }
                float epsilon() const { return EPSILON; }
                VertexFinderMode getFinderMode() const;
                void setFinderMode(VertexFinderMode); 

        // PREDICATS
                bool have(Vertex*) const;
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
                std::list<Vertex*> vertices;
                std::list<Edge*> edges;
                std::list<Face*> faces;
                finderInitialEdge_mode finderInitialEdge = NULL; // pointer to func that looking for initial Edge
	// PRIVATE METHODS
                Face* findContainerOf(Coordinates, Edge* = NULL) const;
                // methods for choose initial Edges. The effectively used is pointed by finderInitialEdge.
                Edge* finderInitial_random() const { 
                        auto it = this->edges.begin();
                        for(unsigned int i = randN(this->edges.size()); i > 0; i--, it++);
                        return *it; 
                }
                Edge* finderInitial_middle() const { 
                        auto it = this->edges.begin();
                        for(unsigned int i = this->edges.size() / 2; i > 0; i--, it++);
                        return *it; 
                }
                Edge* finderInitial_first () const { return this->edges.front(); }
                Edge* finderInitial_last  () const { return this->edges.back(); }
#ifdef DEBUG
                bool applyDelaunayCondition(Face*, unsigned int ttl = 0);
#else
                bool applyDelaunayCondition(Face*);
#endif
                void operateFlip(Edge*);
                // Methods for manipulate lists of components
                /** Destroy and forget given Vertex */
                inline void removeVertexFromVertices(Vertex* v) {
                        delete v;
                        this->vertices.erase(std::find(this->vertices.begin(), this->vertices.end(), v));

                        //for(auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
                                //if((*it) == v) {
                                        //this->vertices.erase(it);
                                        //it = this->vertices.end();
                                        //delete v;
                                        //it--;
                                //}
                        //}
                }
                /** Destroy and forget given Edge */
                inline void removeEdgeFromEdges(Edge* e) {
                        delete e;
                        this->edges.erase(std::find(this->edges.begin(), this->edges.end(), e));
                        //for(auto it = this->edges.begin(); it != this->edges.end(); it++) {
                                //if((*it) == e) {
                                        //this->edges.erase(it);
                                        //delete e;
                                        //it = this->edges.end();
                                        //it--;
                                //}
                        //}
                }
                /** Destroy and forget given Face */
                inline void removeFaceFromFaces(Face* f) {
                        delete f;
                        this->faces.erase(std::find(this->faces.begin(), this->faces.end(), f));
                        //for(auto it = this->faces.begin(); it != this->faces.end(); it++) {
                                //if((*it) == f) {
                                        //this->faces.erase(it);
                                        //delete f;
                                        //it = this->faces.end();
                                        //it--;
                                //}
                        //}
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

                        for(auto f : nei_faces) {
#ifdef DEBUG
                                assert(f != NULL);
#endif
                                this->applyDelaunayCondition(f);
                        }
                }
};



#endif
