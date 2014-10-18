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
Face::Face(Edge* e, bool visible) : edge(e), visible(visible) {
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

        // Deduce coordinates of Circumcenter
        this->circumcenter_ = geometry::circumcenterOf(*this->getP1(), *this->getP2(), *this->getP3());
}



/**
 * @param c tested Coordinates
 * @return true if this coollide at given coordinates
 */
bool Face::collideAt(Coordinates c) {
        return geometry::pointInTriangle(*this->getP1(), *this->getP2(), *this->getP3(), c);
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




