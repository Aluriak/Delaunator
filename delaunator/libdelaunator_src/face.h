#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "geometry.h"
#include "vertex.h"
#include "edge.h"


/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Edge;



/**
 * Face object defined a geometric triangle, with 3 Vertex, 3 Edges, and a geometric centroid.
 * Each Face have a unique ID.
 */
class Face {
	public:
	// CONSTRUCTOR
		Face(Edge*, bool=true);
		~Face();
	// PUBLIC METHODS
                void computeInternalValues();
                bool collideAt(Coordinates);
                bool circumcircleContainCoords(Coordinates) const;
	// ACCESSORS
                //getter
                bool isVisible()      const { return this->visible; }
                Edge* getEdge()       const { return this->edge; }
                Edge* getEdge1()      const;
                Edge* getEdge2()      const;
                Edge* getEdge3()      const;
                Vertex* getP1()       const;
                Vertex* getP2()       const;
                Vertex* getP3()       const;
                unsigned int getID()  const { return this->id; }
                Coordinates circumcenter() const {
                        return this->circumcenter_;
                }
                float circumcircleRadius() const {
                        return this->circumcenter().distanceTo(*this->getP1());
                }
                //setter
                void setEdge(Edge*);
                void setVisibility(bool v)      { this->visible = v; }
	private:
	// ATTRIBUTES
                Edge* edge; // any edge that describe that face
                bool visible; // in a graphical representation
                Coordinates centroid, circumcenter_;
                unsigned int id;
                static unsigned int last_id;
	// PRIVATE METHODS
};


// EXTERNAL METHODS
#if !SWIG
        std::ostream& operator<<(std::ostream&, Face const &);
#endif

#endif
