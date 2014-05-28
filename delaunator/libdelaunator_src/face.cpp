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
        this->centroid.setX((p1->x() + p2->x() + p3->x()) / 3.);
        this->centroid.setY((p1->y() + p2->y() + p3->y()) / 3.);
}



/*
 * COLLIDE AT
 */
// inlines function for slighting main collideAt function
// Algorithm found on http://totologic.blogspot.fr/2014/01/accurate-point-in-triangle-test.html
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
// Return true if this coollide at given coordinates.
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
        // A B C        0 1 2
        // D E F        3 4 5
        // G H I        6 7 8
        // Translate in:
        // p1.x() - p0.x()      p1.y() - p0.y()     (p1.x()*p1.x()-p0.x()*p0.x()) + (p1.y()*p1.y()-p0.y()*p0.y())
        // p2.x() - p0.x()      p2.y() - p0.y()     (p2.x()*p2.x()-p0.x()*p0.x()) + (p2.y()*p2.y()-p0.y()*p0.y())
        // p3.x() - p0.x()      p3.y() - p0.y()     (p3.x()*p3.x()-p0.x()*p0.x()) + (p3.y()*p3.y()-p0.y()*p0.y())
        // If determinant of this matrix is < 0, p is in circumcircle of t
        // determinant: AEI + BFG + CDH - AFH - BDI - CEG
        //float val[9] = {
              //p1.x() - p0.x(),   p1.y() - p0.y(),   (p1.x()*p1.x()-p0.x()*p0.x()) + (p1.y()*p1.y()-p0.y()*p0.y()),
              //p2.x() - p0.x(),   p2.y() - p0.y(),   (p2.x()*p2.x()-p0.x()*p0.x()) + (p2.y()*p2.y()-p0.y()*p0.y()),
              //p3.x() - p0.x(),   p3.y() - p0.y(),   (p3.x()*p3.x()-p0.x()*p0.x()) + (p3.y()*p3.y()-p0.y()*p0.y())
        //};
        //for(float v : val) 
                 //v = round_float(v, EPSILON*10.);


        //float d = (    val[0] * val[4] * val[8] // AEI
                     //+ val[1] * val[5] * val[6] // BFG
                     //+ val[2] * val[3] * val[7] // CDH
                     //- val[0] * val[5] * val[7] // AFH
                     //- val[1] * val[3] * val[8] // BDI
                     //- val[2] * val[4] * val[6] // CEG
                  //);
        float d = (    
        // AEI
          (p1.x()-p0.x()) * (p2.y()-p0.y()) * ((p3.x()*p3.x()-p0.x()*p0.x()) + (p3.y()*p3.y()-p0.y()*p0.y())) 
        // BFG
        + (p1.y()-p0.y()) * ((p2.x()*p2.x()-p0.x()*p0.x()) + (p2.y()*p2.y()-p0.y()*p0.y())) * (p3.x()-p0.x()) 
        // CDH
        + ((p1.x()*p1.x()-p0.x()*p0.x()) + (p1.y()*p1.y()-p0.y()*p0.y())) * (p2.x()-p0.x()) * (p3.y()-p0.y()) 
        // AFH
        - (p1.x()-p0.x()) * ((p2.x()*p2.x()-p0.x()*p0.x()) + (p2.y()*p2.y()-p0.y()*p0.y())) * (p3.y()-p0.y()) 
        // BDI
        - (p1.y()-p0.y()) * (p2.x()-p0.x()) * ((p3.x()*p3.x()-p0.x()*p0.x()) + (p3.y()*p3.y()-p0.y()*p0.y())) 
        // CEG
        - ((p1.x()*p1.x()-p0.x()*p0.x()) + (p1.y()*p1.y()-p0.y()*p0.y())) * (p2.y()-p0.y()) * (p3.x()-p0.x()) 
        );

#if DEBUG
        assert(fabs(d) > -1); // According to Murphy's law, it will happens.
#endif
        return d < (-2. * EPSILON); // must be certain that p0 is NOT ON circumcircle.

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




