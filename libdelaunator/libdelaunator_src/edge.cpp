#include "edge.h"

unsigned int Edge::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
Edge::Edge(Vertex* orgn, Edge* oppst, Edge* nxt_lft, Face* fc, bool visible) :
        visible(visible), origin_vertex(orgn), opposite_edge(oppst), next_left_edge(nxt_lft), left_face(fc) {

        this->id = this->last_id++;
        // Actualise values of face and vertex
        if(this->origin_vertex != NULL) this->origin_vertex->setEdge(this);
        if(this->opposite_edge != NULL) this->opposite_edge->opposite_edge = this;
        if(this->left_face != NULL)     this->left_face->setEdge(this);
}

Edge::~Edge() {
}


/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * DISTANCE TO
 */
// Return distance between the line equivalent to this and given coordinates.
float Edge::distanceTo(Coordinates c) {
	float d = 0.;
	Coordinates A = *(this->originVertex()), B = *(this->destinVertex());
        Coordinates D, T;

        if(A == B) 
                d = c.distanceTo(A);
        else {
                D.setX(B.x() - A.x());
                D.setY(B.y() - A.y());
                T.setX(c.x() - A.x());
                T.setY(c.y() - A.y());
                d = D.x()*T.y() - D.y()*T.x();
        }

	// Around d if close to zero
	if(fabs(d) < EPSILON) d = 0; 

	return d;
}



/*
 * COORDINATES ON THE [STRICT] RIGHT/LEFT
 */
// Return true if given coordinates are on the right of this Edge.
// If strict, coords can't be on the Edge or ahead.
bool Edge::coordOnTheStrictRight(Coordinates c) { return this->distanceTo(c) >  0; }
bool Edge::coordOnTheRight(Coordinates c)       { return this->distanceTo(c) >= 0; }
bool Edge::coordOnTheStrictLeft(Coordinates c)  { return this->distanceTo(c) <  0; }
bool Edge::coordOnTheLeft(Coordinates c)        { return this->distanceTo(c) <= 0; }






/***************************************************
 * ACCESSORS
 ***************************************************/


/***************************************************
 * PRIVATE METHODS
 ***************************************************/



/***************************************************
 * EXTERNAL METHODS
 ***************************************************/
#if !SWIG
std::ostream& operator<<(std::ostream& flux, Edge const &c) {
        flux << "(" << c.getID() << ";" << c.originVertex()->x() << ";" << c.originVertex()->y() << "/";
        flux << c.destinVertex()->x() << ";" << c.destinVertex()->y() << ")";
        return flux;
}
#endif
