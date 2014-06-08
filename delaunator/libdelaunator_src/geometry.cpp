#include "geometry.h"




/***************************************************
 * POINT IN CIRCUMCIRCLE OF 
 ***************************************************/
bool geometry::pointInCircumcircleOf(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0) {
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
 * DISTANCE BETWEEN POINT AND SEGMENT
 ***************************************************/
float geometry::distanceBetweenPointAndSegment(float x1, float x2, float y1, float y2, float px, float py) {
        // Algorithm found on http://totologic.blogspot.fr/2014/01/accurate-point-in-triangle-test.html
        float p1_p2_square_dist = (x2-x1) * (x2-x1) + (y2-y1) * (y2-y1);
        float dotProduct = ((px-x1) * (x2-x1) + (py-y1) * (y2-y1)) / p1_p2_square_dist;
        float distance = 0.;

        if(dotProduct < 0)
                distance = (px-x1) * (px-x1) + (py-y1) * (py-y1);
        else if(dotProduct <= 1) {
                float p_p1_square_dist = (x1-px) * (x1-px) + (y1-py) * (y1-py);
                distance = p_p1_square_dist - dotProduct * dotProduct * p1_p2_square_dist;
        }
        else {  distance = (px-x2) * (px-x2) + (py-y2) * (py-y2); }

        return distance;
}




