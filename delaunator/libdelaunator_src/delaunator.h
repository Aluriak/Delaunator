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
                VirtualVertex* addVirtualVertex(Coordinates, VirtualVertex* = NULL);
                VirtualVertex* addVirtualVertex(float, float, VirtualVertex* = NULL);
                void           delVirtualVertex(VirtualVertex*);
                VirtualVertex* movVirtualVertex(VirtualVertex*, Coordinates);
                VirtualVertex* movVirtualVertex(VirtualVertex*, float, float);
                // object probing
                VirtualVertex* virtualVertexAt(Coordinates,  float=EPSILON) const;
                VirtualVertex* virtualVertexAt(float, float, float=EPSILON) const;
                std::list<VirtualVertex*> virtualVerticesAt(Coordinates,  float=EPSILON) const;
                std::list<VirtualVertex*> virtualVerticesAt(float, float, float=EPSILON) const;
                // frees
                void clear();
                void freeAll();
	// ACCESSORS
                /** @return total number of VirtualVertex in Delaunator */
                unsigned int objectCount() const { return this->object_count; }
                float getXmin() const { return this->triangulation->getXmin(); }
                float getXmax() const { return this->triangulation->getXmax(); }
                float getYmin() const { return this->triangulation->getYmin(); }
                float getYmax() const { return this->triangulation->getYmax(); }
                float epsilon() const { return EPSILON; }
                float distanceMax() const;
                //TriangulationVertexFinderMode getFinderMode() const;
                //void setFinderMode(TriangulationVertexFinderMode); 
                std::list<VirtualVertex*> virtualVertices() const;

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





// PRIVATE
	private:
	// ATTRIBUTES
                Triangulation* triangulation;
                unsigned int object_count;
	// PRIVATE METHODS
};



#endif
