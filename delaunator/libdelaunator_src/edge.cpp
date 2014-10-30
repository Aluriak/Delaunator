#include "edge.h"

unsigned int Edge::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Given references to other objects, if not NULL, will be
 * modified as possible. (calls to accessors setEdge)
 * @param orgn origin Vertex of this Edge
 * @param oppst opposite Edge
 * @param nxt_left next left Edge
 * @param Face delimited by this Edge
 * @param visible necessary for quad-edge representation, and be only set to true if user know exactly what he do
 * @return a new and well initialized Edge
 */
Edge::Edge(Vertex* orgn, Edge* oppst, Edge* nxt_lft, Face* fc, bool visible) :
        visible(visible), origin_vertex(orgn), opposite_edge(oppst), next_left_edge(nxt_lft), left_face(fc) {

        this->id = this->last_id++;
        // Actualise values of face and vertex
        if(this->origin_vertex != NULL) this->origin_vertex->setEdge(this);
        if(this->opposite_edge != NULL) this->opposite_edge->opposite_edge = this;
        if(this->left_face != NULL)     this->left_face->setEdge(this);
#ifdef FOLLOW_SEARCH
        this->passing = false;
#endif
}

/**
 * Frees.
 */
Edge::~Edge() {
        // nothing to frees
}


/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/**
 * @param x x coordinate of target 
 * @param y y coordinate of target 
 * Return square of distance between the edge and given coordinates.
 */
float Edge::squareDistanceTo(float px, float py) const {
        return geometry::squareDistanceBetweenSegmentAndPoint(
                        this->origin_vertex->x(), this->origin_vertex->y(), 
                        this->destinVertex()->x(), this->destinVertex()->y(), 
                        px, py
        );
}



/*
 * Inline function used by next Predicats.
 * Return dot product of given edge and point defined by given coordinates.
 */
inline float dot_product(const Edge* const edge, const Coordinates c) {
         return (edge->destinVertex()->y() - edge->originVertex()->y())
                 * (c.x() - edge->originVertex()->x()) 
                +  (-edge->destinVertex()->x() + edge->originVertex()->x())
                 * (c.y() - edge->originVertex()->y());
}
/**
 * @param c evaluated Coordinates
 * @return true if given coordinates are on the right of this Edge, but not on the Edge itself aligned with.
 */
bool Edge::coordOnTheStrictRight(Coordinates c) const { return dot_product(this, c) <  0.; }
/**
 * @param c evaluated Coordinates
 * @return true if given coordinates are on the right of this Edge.
 */
bool Edge::coordOnTheRight(Coordinates c)       const { return dot_product(this, c) <= 0.; }
/**
 * @param c evaluated Coordinates
 * @return true if given coordinates are on the left of this Edge, but not on the Edge itself aligned with.
 */
bool Edge::coordOnTheStrictLeft(Coordinates c)  const { return dot_product(this, c) >  0.; }
/**
 * @param c evaluated Coordinates
 * @return true if given coordinates are on the left of this Edge.
 */
bool Edge::coordOnTheLeft(Coordinates c)        const { return dot_product(this, c) >= 0.; }






/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * An external Edge is an Edge that rely two corner Vertex.
 * @return true iff this is one of the eight external Edge.
 */
bool Edge::isExternal() const {
        // an edge is external if in contact with a unvisible face. 
        return (!this->leftFace()->isVisible()) || (!this->rightFace()->isVisible());
}




/**
 * @return distance between the origin and destination Vertex
 */
float Edge::length() const {
        return this->origin_vertex->distanceTo(*this->opposite_edge->origin_vertex);
}

/**
 * @return square distance between the origin and destination Vertex
 */
float Edge::squareLength() const {
        return this->origin_vertex->squareDistanceTo(*this->opposite_edge->origin_vertex);
}

/**
 * @return Coordinates of the middle of this Edge.
 */
Coordinates Edge::middle() const {
        return (*this->originVertex() + *this->destinVertex()) / 2;
}
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
