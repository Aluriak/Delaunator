#include "face.h"
#include "iterators.h"

unsigned int Face::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Creat a new Face.
 * @param e an Edge that is a side of Face. Its internal Face reference will be modified
 * @param visible necessary for quad-edge representation, and be only set to true if user know exactly what he do
 * @return a new and well initialized Face
 */
Face::Face(Edge* e, bool visible) : visible(visible), edge(e) {
        this->id = this->last_id++;
        if(e != NULL) {
                this->computeInternalValues();
                e->setLeftFace(this);
        }
}

/**
 * Frees.
 */
Face::~Face() {
        // nothing to frees
}


/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/**
 * Compute values that changed if Vertex are modified
 */
void Face::computeInternalValues() {
        // Get coordinates of the three points of this.
        Vertex *p1 = this->getP1(), *p2 = this->getP2(), *p3 = this->getP3();
        
        // Deduce coordinates of Centroid.
        this->centroid.setX((p1->x() + p2->x() + p3->x()) / 3.);
        this->centroid.setY((p1->y() + p2->y() + p3->y()) / 3.);
}



/**
 * inlines function for slighting Face::collideAt function
 * Algorithm found on http://totologic.blogspot.fr/2014/01/accurate-point-in-triangle-test.html
 */
inline float collideAt_side(float x1, float y1, float x2, float y2, float x, float y) {
         return (y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1);
}
inline float collideAt_distanceSquarePointToSegment(float x1, float y1, float x2, float y2, float x, float y) {
        float p1_p2_square_dist = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
        float dotProduct = ((x - x1)*(x2 - x1) + (y - y1)*(y2 - y1)) / p1_p2_square_dist;
        if(dotProduct < 0)
                return (x - x1)*(x - x1) + (y - y1)*(y - y1);
        else if(dotProduct <= 1) {
                float p_p1_squareLength = (x1 - x)*(x1 - x) + (y1 - y)*(y1 - y);
                return p_p1_squareLength - dotProduct * dotProduct * p1_p2_square_dist;
        }
        else
                return (x - x2)*(x - x2) + (y - y2)*(y - y2);
}
/**
 * @param c tested Coordinates
 * @return true if this coollide at given coordinates
 */
bool Face::collideAt(Coordinates c) {
        bool collide = false;
        float x1 = this->getP1()->x(), y1 = this->getP1()->y();
        float x2 = this->getP2()->x(), y2 = this->getP2()->y();
        float x3 = this->getP3()->x(), y3 = this->getP3()->y();
        float x = c.x(), y = c.y();
        float xmin = min(x1, min(x2, x3)) - EPSILON;
        float xmax = max(x1, max(x2, x3)) + EPSILON;
        float ymin = min(y1, min(y2, y3)) - EPSILON;
        float ymax = max(y1, max(y2, y3)) + EPSILON;

        if(xmin <= x && x <= xmax && ymin <= y && y <= ymax) {
                bool checkside1 = collideAt_side(x1, y1, x2, y2, x, y) >= 0;
                bool checkside2 = collideAt_side(x2, y2, x3, y3, x, y) >= 0;
                bool checkside3 = collideAt_side(x3, y3, x1, y1, x, y) >= 0;
                if(checkside1 && checkside2 && checkside3) {
                        collide = true;
                } else {
                        collide = collideAt_distanceSquarePointToSegment(x1, y1, x2, y2, x, y) <= EPSILON*EPSILON;
                        collide |= collideAt_distanceSquarePointToSegment(x2, y2, x3, y3, x, y) <= EPSILON*EPSILON;
                        collide |= collideAt_distanceSquarePointToSegment(x3, y3, x1, y1, x, y) <= EPSILON*EPSILON;
                }
        }
        return collide;
}





/**
 * @param p0 Coordinates of tested point
 * @return true if given Coordinates are in circumcircle of this Face
 */
bool Face::circumcircleContainCoords(Coordinates p0) const {
        return geometry::pointInCircumcircleOf(
                        *(this->getP1()), 
                        *(this->getP2()), 
                        *(this->getP3()), 
                        p0
        );
}




/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * @return a Vertex that compose this Face.
 */
Vertex* Face::getP1()  const  { return this->edge->originVertex(); }
/**
 * @return a Vertex that compose this Face.
 */
Vertex* Face::getP2()  const  { return this->edge->nextLeftEdge()->originVertex(); }
/**
 * @return a Vertex that compose this Face.
 */
Vertex* Face::getP3()  const  { return this->edge->nextLeftEdge()->nextLeftEdge()->originVertex(); }

/**
 * @return an Edge that is linked to this Face, and have for next left Edge the Edge returned by Face::getEdge2() method
 */
Edge* Face::getEdge1() const  { return this->edge; }
/**
 * @return an Edge that is linked to this Face, and have for next left Edge the Edge returned by Face::getEdge3() method
 */
Edge* Face::getEdge2() const  { return this->edge->nextLeftEdge(); }
/**
 * @return an Edge that is linked to this Face, and have for next left Edge the Edge returned by Face::getEdge1() method
 */
Edge* Face::getEdge3() const  { return this->edge->nextLeftEdge()->nextLeftEdge(); }

/**
 * Set received Edge has linked Edge for this Face.
 * @param e received Edge
 */
void Face::setEdge(Edge* e) { 
        this->edge = e; 
        if(e != NULL) { 
                this->edge->setLeftFace(this);
                this->computeInternalValues(); 
        }
}


/***************************************************
 * PRIVATE METHODS
 ***************************************************/






/***************************************************
 * EXTERNAL METHODS
 ***************************************************/
#if !SWIG
std::ostream& operator<<(std::ostream& flux, Face const &c) {
        flux << "{";
        for(IteratorFaceToVertices it(&c); it != it.end(); it++) 
                flux << " " << **it;
        flux << " }";
        return flux;
}
#endif




