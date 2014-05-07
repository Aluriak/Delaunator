#include "face.h"
#include "iterators.h"

unsigned int Face::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
Face::Face(Edge* e, bool visible) : visible(visible), edge(e) {
        this->id = this->last_id++;
        if(e != NULL) {
                this->computeInternalValues();
                e->setLeftFace(this);
        }

}

Face::~Face() {
}


/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * COMPUTE INTERNAL VALUES
 */
// Compute values that changed if vertex are modified
void Face::computeInternalValues() {
        // Get coordinates of the three points of this.
        Vertex *p1 = this->getP1(), *p2 = this->getP2(), *p3 = this->getP3();
        
        // Deduce coordinates of Centroid.
        this->centroid.x = (p1->x + p2->x + p3->x) / 3.;
        this->centroid.y = (p1->y + p2->y + p3->y) / 3.;
}



/*
 * COLLIDE AT
 */
// Return true if this coollide at given coordinates.
bool Face::collideAt(Coordinates c) {
        bool collide = true;
        IteratorFaceToEdges it(this);
        for(; collide && it != it.end(); it++) {
                if((*it)->coordOnTheRight(c)) 
                        collide = false;
        }
        return collide;
}




/*
 * CIRCUMCIRCLE CONTAIN VERTEX/
 */
// Return true if given point is in circumcircle of this Face.
bool Face::circumcircleContainCoords(Coordinates p0) const {
        Coordinates p1 = *(this->getP1());
        Coordinates p2 = *(this->getP2());
        Coordinates p3 = *(this->getP3());

        // Algorithm found here: https://en.wikipedia.org/wiki/Delaunay_triangulation#Algorithms
        // matrix:
        // A B C
        // D E F
        // G H I
        // Trnaslate in:
        // p1.x - p0.x          p1.y - p0.y             (p1.x*p1.x-p0.x*p0.x) + (p1.y*p1.y-p0.y*p0.y)
        // p2.x - p0.x          p2.y - p0.y             (p2.x*p2.x-p0.x*p0.x) + (p2.y*p2.y-p0.y*p0.y)
        // p3.x - p0.x          p3.y - p0.y             (p3.x*p3.x-p0.x*p0.x) + (p3.y*p3.y-p0.y*p0.y)
        // If determinant of this matrix is < 0, p is in circumcircle of t
        // determinant: AEI + BFG + CDH - AFH - BDI - CEG
        // p0: tested 
        float d = (    (p1.x - p0.x) * (p2.y - p0.y) * ((p3.x*p3.x-p0.x*p0.x) + (p3.y*p3.y-p0.y*p0.y)) // AEI
                     + (p1.y - p0.y) * ((p2.x*p2.x-p0.x*p0.x) + (p2.y*p2.y-p0.y*p0.y)) * (p3.x - p0.x) // BFG
                     + ((p1.x*p1.x-p0.x*p0.x) + (p1.y*p1.y-p0.y*p0.y)) * (p2.x - p0.x) * (p3.y - p0.y) // CDH
                     - (p1.x - p0.x) * ((p2.x*p2.x-p0.x*p0.x) + (p2.y*p2.y-p0.y*p0.y)) * (p3.y - p0.y) // AFH
                     - (p1.y - p0.y) * (p2.x - p0.x) * ((p3.x*p3.x-p0.x*p0.x) + (p3.y*p3.y-p0.y*p0.y)) // BDI
                     - ((p1.x*p1.x-p0.x*p0.x) + (p1.y*p1.y-p0.y*p0.y)) * (p2.y - p0.y) * (p3.x - p0.x) // CEG
                  );

#if DEBUG
        assert(fabs(d) > -1); // According to Murphy's law, it will happens.
#endif
        return d < (-EPSILON);

}




/***************************************************
 * ACCESSORS
 ***************************************************/
Vertex* Face::getP1()  const  { return this->edge->originVertex(); }
Vertex* Face::getP2()  const  { return this->edge->nextLeftEdge()->originVertex(); }
Vertex* Face::getP3()  const  { return this->edge->nextLeftEdge()->nextLeftEdge()->originVertex(); }

Edge* Face::getEdge1() const  { return this->edge; }
Edge* Face::getEdge2() const  { return this->edge->nextLeftEdge(); }
Edge* Face::getEdge3() const  { return this->edge->nextLeftEdge()->nextLeftEdge(); }

/*
 * SET EDGE
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




