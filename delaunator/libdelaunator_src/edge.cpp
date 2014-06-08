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
float Edge::squareDistanceTo(float x, float y) const {
        // Algorithm found on http://totologic.blogspot.fr/2014/01/accurate-point-in-triangle-test.html
        float x1 = this->origin_vertex->x(), y1 = this->origin_vertex->y();
        float x2 = this->destinVertex()->x(), y2 = this->destinVertex()->y();

        float p1_p2_square_dist = (x2-x1) * (x2-x1) + (y2-y1) * (y2-y1);
        float dotProduct = ((x-x1) * (x2-x1) + (y-y1) * (y2-y1)) / p1_p2_square_dist;
        float distance = 0.;

        if(dotProduct < 0)
                distance = (x-x1) * (x-x1) + (y-y1) * (y-y1);
        else if(dotProduct <= 1) {
                float p_p1_square_dist = (x1-x) * (x1-x) + (y1-y) * (y1-y);
                distance = p_p1_square_dist - dotProduct * dotProduct * p1_p2_square_dist;
        }
        else
                distance = (x-x2) * (x-x2) + (y-y2) * (y-y2);

        return distance;
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
