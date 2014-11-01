#ifndef DELAUNATOR_MAIN_INTERFACE_H_INCLUDED
#define DELAUNATOR_MAIN_INTERFACE_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "triangulation.h"



/*
 * DEFINES
 */



/*
 * PREDECLARATIONS
 */






/**
 * Delaunay class definition.  
 * Provide iterators, access to VirtualVertexs…
 * Have all methods for add, delete and move Objects.
 */
class Delaunator {
	public:
	// CONSTRUCTOR
		Delaunator(const float, const float, 
                           const float, const float, 
                           const VertexFinderMode = 
                                   VERTEX_FINDER_MODE_LAST);
		~Delaunator();
	// PUBLIC METHODS
                // modify triangulation
                VirtualVertex* addVirtualVertex(Coordinates);
                VirtualVertex* addVirtualVertex(float, float);
                void           delVirtualVertex(VirtualVertex*);
                VirtualVertex* movVirtualVertex(VirtualVertex*, Coordinates);
                VirtualVertex* movVirtualVertex(VirtualVertex*, float, float);
                // object probing
                VirtualVertex* virtualVertexAt(Coordinates,  float=EPSILON) const;
                VirtualVertex* virtualVertexAt(float, float, float=EPSILON) const;
                std::list<VirtualVertex*> virtualVerticesAt(Coordinates,  float=EPSILON) const;
                std::list<VirtualVertex*> virtualVerticesAt(float, float, float=EPSILON) const;
	// ACCESSORS
                /** @return total number of VirtualVertex in Delaunator */
                unsigned int objectCount() const { return this->object_count; }
                float getXmin() const { return this->triangulation->getXmin(); }
                float getXmax() const { return this->triangulation->getXmax(); }
                float getYmin() const { return this->triangulation->getYmin(); }
                float getYmax() const { return this->triangulation->getYmax(); }
                float epsilon() const { return EPSILON; }
                //TriangulationVertexFinderMode getFinderMode() const;
                //void setFinderMode(TriangulationVertexFinderMode); 

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
                std::list<VirtualVertex*> virtualVertices() const;
                //IteratorVertexToNeighbourVertices getNeighbors(Vertex*);
                //// edges necessary for user
                //IteratorOnEdges iterEdges()             { return IteratorOnEdges(&this->edges); }
                //IteratorOnEdges_read iterEdges_read() const
                                                        //{ return IteratorOnEdges_read(&this->edges); }
                //// all edges, including the externals ones
                //IteratorOnAllEdges iterAllEdges()       { return IteratorOnAllEdges(&this->edges); }
                //IteratorOnAllEdges_read iterAllEdges_read() const
                                                        //{ return IteratorOnAllEdges_read(&this->edges); }
                //// faces that are visible to user
                //IteratorOnFaces iterFaces()             { return IteratorOnFaces(&this->faces); }
                //IteratorOnFaces_read iterFaces_read() const
                                                        //{ return IteratorOnFaces_read(&this->faces); }
                //// all faces, including the unvisible ones
                //IteratorOnAllFaces iterAllFaces()       { return IteratorOnAllFaces(&this->faces); }
                //IteratorOnAllFaces_read iterAllFaces_read() const
                                                        //{ return IteratorOnAllFaces_read(&this->faces); }
                //// vertices placed by user
                //IteratorOnVertices iterVertices()       { return IteratorOnVertices(&this->vertices); }
                //IteratorOnVertices_read iterVertices_read() const 
                                                        //{ return IteratorOnVertices_read(&this->vertices); }
                //// all vertices, including the 4 used for create and maintain the mesh
                //IteratorOnAllVertices iterAllVertices() { return IteratorOnAllVertices(&this->vertices); }
                //IteratorOnAllVertices_read iterAllVertices_read() const 
                                                        //{ return IteratorOnAllVertices_read(&this->vertices); }




// PRIVATE
	private:
	// ATTRIBUTES
                Triangulation* triangulation;
                unsigned int object_count;
	// PRIVATE METHODS
};



#endif
