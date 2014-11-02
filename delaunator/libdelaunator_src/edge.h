#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "vertex.h"
#include "face.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Vertex;
class Face;




/**
 * Edge is the main object of quad-edge representation. 
 */
class Edge {
	public:
	// CONSTRUCTOR
		Edge(Vertex* = NULL, Edge* = NULL, Edge* = NULL, Face* = NULL, bool = true);
		~Edge();
	// PUBLIC METHODS
                /**
                 * @param c target Coordinates
                 * @return shorter distance between this Edge and the target
                 */
                float distanceTo(Coordinates c)       const { return sqrt(this->squareDistanceTo(c.x(), c.y())); }
                /**
                 * @param x x coordinate of target 
                 * @param y y coordinate of target 
                 * @return shorter distance between this Edge and the target
                 */
                float distanceTo(float x, float y)    const { return sqrt(this->squareDistanceTo(x, y)); }
                /**
                 * @param c target Coordinates
                 * @return square of shorter distance between this Edge and the target
                 */
                float squareDistanceTo(Coordinates c) const { return this->squareDistanceTo(c.x(), c.y()); }
                /**
                 * @param x x coordinate of target 
                 * @param y y coordinate of target 
                 * @return square of shorter distance between this Edge and the target
                 */
                float squareDistanceTo(float x, float y) const;
                bool coordOnTheStrictRight(Coordinates)  const;
                bool coordOnTheRight(Coordinates)        const;
                bool coordOnTheStrictLeft(Coordinates)   const;
                bool coordOnTheLeft(Coordinates)         const;
	// ACCESSORS
                // getter
                Vertex* originVertex()  const { return this->origin_vertex; }
                Vertex* destinVertex() 	const { return this->opposite_edge->origin_vertex; }

                Edge* oppositeEdge() 	const { return this->opposite_edge; }
                Edge* nextLeftEdge() 	const { return this->next_left_edge; }
                Edge* prevLeftEdge() 	const { return this->next_left_edge->next_left_edge; }
                Edge* nextRightEdge()   const { return this->
                                                opposite_edge->next_left_edge->next_left_edge->opposite_edge;}
                Edge* prevRightEdge() 	const { return this->opposite_edge->next_left_edge->opposite_edge; }
                Edge* rotLeftEdge() 	const { return this->next_left_edge->next_left_edge->opposite_edge; }
                Edge* rotRightEdge() 	const { return this->opposite_edge->next_left_edge; }
                Face* leftFace() 	const { return this->left_face; }
                Face* rightFace() 	const { return this->opposite_edge->left_face; }

                bool isVisible()        const { return this->visible; }
                bool isExternal()       const;
                unsigned int getID()    const { return this->id; }
                float length()          const;
                float squareLength()    const;

                Coordinates middle()    const;
                // setters
                void setOriginVertex(Vertex* v) { this->origin_vertex = v; v->setEdge(this); }
                void setOppositeEdge(Edge* e)   { this->opposite_edge = e; }
                void setNextLeftEdge(Edge* e)   { this->next_left_edge = e; }
                void setLeftFace(Face* f, short ttl = 2) { // time to live permit an update on all sides of face.
                        this->left_face = f; 
                        if(ttl > 0)
                                this->nextLeftEdge()->setLeftFace(f, ttl-1);
                }
                void setVisibility(bool v)      { this->visible = v; }
#ifdef FOLLOW_SEARCH
                bool passing;
#endif
	private:
	// ATTRIBUTES
                bool visible; // in a graphical representation
                Vertex* origin_vertex;
                Edge* opposite_edge;
                Edge* next_left_edge;
                Face* left_face;
                unsigned int id;
                static unsigned int last_id;
	// PRIVATE METHODS
};


// EXTERNAL METHODS
#if !SWIG
        std::ostream& operator<<(std::ostream&, Edge const &);
#endif

#endif
