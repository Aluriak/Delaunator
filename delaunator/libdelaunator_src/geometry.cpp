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
 * CIRCUMCENTER OF
 ***************************************************/
Coordinates geometry::circumcenterOf(Coordinates A, Coordinates B, Coordinates C) {
        // Source: wikipedia.org/wiki/Circumscribed_circle#Cartesian_coordinates
        float D = 2*(A.x()*(B.y() - C.y()) + B.x()*(C.y() - A.y()) + C.x()*(A.y() - B.y()));
        float factA = A.x()*A.x() + A.y()*A.y();
        float factB = B.x()*B.x() + B.y()*B.y();
        float factC = C.x()*C.x() + C.y()*C.y();
        Coordinates center;
        center.setX(  (factA*(B.y()- C.y()) + factB*(C.y() - A.y()) + factC*(A.y() - B.y()))  / D);
        center.setY(  (factA*(C.x()- B.x()) + factB*(A.x() - C.x()) + factC*(B.x() - A.x()))  / D);
        return center;
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








/***************************************************
 * INTERSECTION OF LINES
 ***************************************************/
bool geometry::parallelsLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        // source: https://en.wikipedia.org/wiki/Line-line_intersection#Mathematics
        return fabs((A.x()-B.x()) * (C.y()-D.y())  -  (A.y()-B.y()) * (C.x()-D.x())) < EPSILON;
}








/***************************************************
 * INTERSECTION OF LINES
 ***************************************************/
Coordinates geometry::intersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        assert(!geometry::parallelsLines(A,B,C,D));
        Coordinates intersec;
        float det = (A.x()-B.x()) * (C.y()-D.y())  -  (A.y()-B.y()) * (C.x()-D.x());
        float fA = A.x()*B.y() - A.y()*B.x();
        float fB = C.x()*D.y() - C.y()*D.x();
        intersec.setX((fA*(C.x()-D.x()) - (A.x()-B.x())*fB) / det);
        intersec.setY((fA*(C.y()-D.y()) - (A.y()-B.y())*fB) / det);
        return intersec;
}








/***************************************************
 * COLLISION BETWEEN SEGMENT AND LINE
 ***************************************************/
bool geometry::collisionBetweenSegmentAndLine(Coordinates O, Coordinates P, Coordinates A, Coordinates B) {
        // 0,1    1,0   0,0     2,2
        float AB_x = B.x() - A.x();  // 2
        float AB_y = B.y() - A.y();  // 2
        float AP_x = P.x() - A.x();  // 1
        float AP_y = P.y() - A.y();  // 0
        float AO_x = O.x() - A.x();  // 0
        float AO_y = O.y() - A.y();  // 1
        // -2 * 2
        return (AB_x*AP_y - AB_y*AP_x)*(AB_x*AO_y - AB_y*AO_x) <= 0;
}







/***************************************************
 * COLLISION BETWEEN SEGMENT AND SEGMENT
 ***************************************************/
bool geometry::collisionBetweenSegmentAndSegment(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        return collisionBetweenSegmentAndLine(A, B, C, D) && collisionBetweenSegmentAndLine(C, D, A, B);
}








/***************************************************
 * COLLISION BETWEEN LINE AND CIRCLE
 ***************************************************/
bool geometry::collisionBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius) {
        float u_x = B.x() - A.x();
        float u_y = B.y() - A.y();
        float AC_x = C.x() - A.x();
        float AC_y = C.y() - A.y();
        float numerator = fabs(u_x*AC_y - u_y*AC_x); 
        float denominator = sqrt(u_x*u_x + u_y*u_y); 
        return (numerator / denominator) < radius;
}








/***************************************************
 * POINT IN CIRCLE
 ***************************************************/
bool geometry::pointInCircle(Coordinates A, Coordinates C, float radius) {
        return (A.x()-C.x())*(A.x()-C.x()) + (A.y()-C.y())*(A.y()-C.y()) > radius*radius;
}








/***************************************************
 * COLLISION BETWEEN SEGMENT AND CIRCLE
 ***************************************************/
bool geometry::collisionBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius) {
        bool collision = false;
        if(geometry::collisionBetweenLineAndCircle(A, B, C, radius)) {
                float AB_x = B.x() - A.x();
                float AB_y = B.y() - A.y();
                float AC_x = C.x() - A.x();
                float AC_y = C.y() - A.y();
                float BC_x = C.x() - B.x();
                float BC_y = C.y() - B.y();
                float scal1 = AB_x*AC_x + AB_y*AC_y;  // scalar product
                float scal2 = (-AB_x)*BC_x + (-AB_y)*BC_y;  // scalar product
                // I is between A and B, or A or B is in the circle
                collision = (scal1 >= 0 && scal2 >= 0) 
                          || geometry::pointInCircle(A, C, radius)
                          || geometry::pointInCircle(B, C, radius);
        }
        return collision;
}








/***************************************************
 * PROJECTION OF POINT ON LINE
 ***************************************************/
Coordinates geometry::projectionOfPointOnLine(Coordinates A, Coordinates B, Coordinates C) {
        float u_x  = B.x() - A.x(); 
        float u_y  = B.y() - A.y(); 
        float AC_x = C.x() - A.x();
        float AC_y = C.y() - A.y();
        float ti = (u_x*AC_x + u_y*AC_y)/(u_x*u_x + u_y*u_y);
        Coordinates projection(A.x() + ti*u_x, A.y() + ti*u_y);
        return projection;
}








/***************************************************
 * INTERSECTIONS BETWEEN LINE AND CIRCLE
 ***************************************************/
unsigned int geometry::intersectionsBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, 
                float radius, Coordinates* S1, Coordinates* S2) {
#if DEBUG
        assert(S1 != NULL);
        assert(S2 != NULL);
#endif
        float coef, ordn, delta;
        unsigned int nb_solution = 0;
        if(coeffAndOrdnOfLine(A, B, &coef, &ordn)) {
                // line is not vertical
                float cA = 1 + coef*coef; 
                float cB = -2*C.x() + 2*coef*ordn - 2*coef*C.y();
                float cC = -radius*radius + C.x()*C.x() + ordn*ordn + C.y()*C.y() - 2*ordn*C.y();
                delta = cB*cB - 4*cA*cC;

                if(delta >= 0) {
                        S1->setX(((-cB + sqrt(delta)) / (2*cA)));
                        S1->setY(S1->x() * coef + ordn);
                        nb_solution++;
                }
                if(delta > 0) {
                        S2->setX(((-cB + sqrt(delta)) / (2*cA)));
                        S2->setY(S1->x() * coef + ordn);
                        nb_solution++;
                }
        } else {
                // line is vertical
                float cB = -2*C.y();
                float cC = -radius*radius + (ordn - C.x())*(ordn - C.x()) + C.y();
                delta = cB*cB - 4*cC;

                if(delta >= 0) {
                        S1->setX(ordn);
                        S1->setY(((-cB + sqrt(delta)) / 2));
                        nb_solution++;
                }
                if(delta > 0) {
                        S2->setX(ordn);
                        S2->setY(((-cB - sqrt(delta)) / 2));
                        nb_solution++;
                }
        }
        return nb_solution;
}








/***************************************************
 * COEF AND ORDN OF LINE
 ***************************************************/
bool geometry::coeffAndOrdnOfLine(Coordinates A, Coordinates B, float* coef, float* ordn) {
#if DEBUG
        assert(coef != NULL);
        assert(ordn != NULL);
#endif
        bool isVertical = fabs(A.x() - B.x()) < EPSILON;
        if(isVertical) *ordn = A.x();
        else {
                *coef = round_float((B.y() - A.y()) / (B.x() - A.x()), EPSILON*10.);
                *ordn = round_float(A.y() - A.x()*(*coef), EPSILON*10.);
        }
        return not isVertical;
}





