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
 * POINT IN TRIANGLE
 ***************************************************/
/*
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
/*
 * POINT IN TRIANGLE
 */
bool geometry::pointInTriangle(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0) {
        bool collide = false;
        float x1 = p1.x(), y1 = p1.y();
        float x2 = p2.x(), y2 = p2.y();
        float x3 = p3.x(), y3 = p3.y();
        float x  = p0.x(), y  = p0.y();
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




