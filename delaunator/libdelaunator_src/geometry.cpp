#include "geometry.h"



/***************************************************
 * UNIT TESTS
 ***************************************************/
/**
 * call that once for verify integrity of geometry fonctions
 * @warning: assertion fail if detected problem
 */
void geometry::unit_tests() {
        Coordinates A = Coordinates( 0,  0), B = Coordinates( 1, 1);
        Coordinates C = Coordinates(10, 10), D = Coordinates(10, 1);
        std::vector<Coordinates*> coords = {&A, &B, &C};
        assert(pointInTriangle(A, C, D, B)); // B is in ACD
        assert(pointInTriangle(
                    Coordinates( 91,372), Coordinates(314,360), Coordinates(118,204),
                    Coordinates(122,309)
        ));
        assert(pointInTriangle(
                    Coordinates(0,0), Coordinates(2,4), Coordinates(4,0),
                    Coordinates(1,2)
        ));
        assert(pointInTriangle(
                    Coordinates(0,0), Coordinates(2,4), Coordinates(4,0),
                    Coordinates(1,2)
        ));
        assert(pointInTriangle(
                    Coordinates(0,0), Coordinates(0,2), Coordinates(2,0),
                    Coordinates(0,0)
        ));
        // point in triangle ok !

        assert(alignedPoints(A, B, C));
        assert(alignedPoints(A, A, A));
        assert(alignedPoints(A, A, D));
        A = Coordinates(0, 0);
        assert(alignedPoints(A, A, A));
        assert(alignedPoints(B, B, B));
        A = Coordinates(1, 2);
        B = Coordinates(0, 2);
        C = Coordinates(3, 2);
        assert(alignedPoints(A, B, C));
        assert(alignedPoints(B, C, A));
        assert(alignedPoints(A, C, A));
        // aligned point ok !


        A = Coordinates(1, 3);
        B = Coordinates(3, 3);
        C = Coordinates(3, 1);
        assert(geometry::collisionBetweenSegmentAndSegment(B, A, B, C));
        // segment x segment collision ok !


        float result;
        result = geometry::signedPolygonArea(1, A);
        assert(result == 0.);
        result = geometry::signedPolygonArea(2, A, A);
        assert(result == 0.);
        // signed polygon area for one point ok


        assert(       pointInClockwiseOrder(1, Coordinates( 0,  0)));
        assert(pointInCounterClockwiseOrder(1, Coordinates( 0,  0)));
        assert(       pointInClockwiseOrder(1, Coordinates( 1, -1)));
        assert(pointInCounterClockwiseOrder(1, Coordinates(-1,  1)));
        assert(       pointInClockwiseOrder(1, Coordinates(.4,-.1)));
        assert(pointInCounterClockwiseOrder(1, Coordinates(.4,-.1)));
        // {counter,}clockwise order for one point ok


        assert(       pointInClockwiseOrder(3, Coordinates( 0,  0), Coordinates( 0,  0), Coordinates( 0,  0)));
        assert(pointInCounterClockwiseOrder(3, Coordinates( 0,  0), Coordinates( 0,  0), Coordinates( 0,  0)));
        assert(       pointInClockwiseOrder(3, Coordinates( 1,  0), Coordinates( 1,  3), Coordinates( 1,  6)));
        assert(pointInCounterClockwiseOrder(3, Coordinates( 1,  0), Coordinates( 1,  3), Coordinates( 1,  6)));
        assert(       pointInClockwiseOrder(3, Coordinates(-3, 10), Coordinates(-3,  3), Coordinates(-3, -5)));
        assert(pointInCounterClockwiseOrder(3, Coordinates(-3, 10), Coordinates(-3,  3), Coordinates(-3, -5)));
        // {counter,}clockwise order for aligned points ok


        A = Coordinates(26, -1); // (ABC) is in counter clockwise order
        B = Coordinates(-1, 21); // when its assumed that Y axis in "inverted"
        C = Coordinates(26, 21);
        assert(pointInClockwiseOrder(3,
                *coords[0],
                *coords[1],
                *coords[2]
        ) == pointInClockwiseOrder(coords));
        assert(not pointInClockwiseOrder(3,
                *coords[0],
                *coords[1],
                *coords[2]
        ));
        // careful: not strictly equivalent to precedent assertion
        assert(pointInCounterClockwiseOrder(3,
                *coords[0],
                *coords[1],
                *coords[2]
        ) == pointInCounterClockwiseOrder(coords));
        assert(pointInCounterClockwiseOrder(3,
                *coords[0],
                *coords[1],
                *coords[2]
        ));
        assert(not pointInClockwiseOrder(5,
                Coordinates(1, 0),
                Coordinates(1, 5),
                Coordinates(4, 5),
                Coordinates(6, 4),
                Coordinates(5, 0)
        ));
        // pointIn{Counter,}ClockwiseOrder is ok !


        assert(pointInsideCircle(Coordinates(0, 0.9999), Coordinates(0, 0), 1));
        assert(pointInsideCircle(Coordinates(0, 0), Coordinates(0, 0), 1));
        assert(pointOnCircle(Coordinates(1, 0), Coordinates(0, 0), 1));
        assert(pointOnCircle(Coordinates(0, -1), Coordinates(0, 0), 1));
        // PointInsideCircle is ok !


        // Point in circumcircle
        // aligned points
        A = Coordinates(1, 2);
        B = Coordinates(0, 2);
        C = Coordinates(3, 2);
        D = Coordinates(1, -100);
        assert(not pointInCircumcircleOf(A, B, C, D));
        assert(not pointInCircumcircleOf(Coordinates(1,2), Coordinates(0,2),
                                Coordinates(3,2), Coordinates(1,-100)));
        assert(not pointInCircumcircleOf(Coordinates(0,2), Coordinates(1,2),
                                Coordinates(3,2), Coordinates(2,4)));
        assert(not pointInCircumcircleOf(Coordinates(1,2), Coordinates(0,2),
                                Coordinates(3,2), Coordinates(2,4)));

        // non aligned points
        assert(not pointInCircumcircleOf(Coordinates(1,0), Coordinates(1,2),
                                Coordinates(0,1), Coordinates(3-EPSILON,1))
        ); // 3 firsts are in counter-clockwise
        assert(    pointInCircumcircleOf(Coordinates(1,0), Coordinates(1,2),
                                Coordinates(0,1), Coordinates(2-0.1,1))
        ); // 3 firsts are in counter-clockwise
        assert(    pointInCircumcircleOf(Coordinates(1,0), Coordinates(1,2),
                                Coordinates(0,1), Coordinates(1-EPSILON,1))
        ); // 3 firsts are in counter-clockwise
        float d = sqrt(squareDistanceBetweenSegmentAndPoint(
                0,0, 1,1, 1,2
        ));
        assert(abs(d-1.) < EPSILON);
        // PointInCircumcircle is ok !


        // study Circle {C; 3} and Line (A;B) with A (-7;7) and B(5;7) and C (3;6)
        Coordinates inter1, inter2;
        A = Coordinates(-7, 7);
        B = Coordinates(5, 7);
        C = Coordinates(3, 6);
        float radius = 3, square_radius = radius*radius;
        assert(geometry::intersectionsBetweenLineAndCircle(
                                A, B, // segment [AB]
                                C, square_radius, // circle center and squared radius
                                &inter1, &inter2  // solutions
                ) == 2
        );
        // horizontal lines are ok !


        // study Circle {C; 2} and Line (A;B) with A (6;8) and B (6;2) and C (4;6)
        A = Coordinates(6, 8);
        B = Coordinates(6, 2);
        C = Coordinates(4, 6);
        radius = 2;
        square_radius = radius*radius;
        assert(square_radius == 4);
        assert(geometry::intersectionsBetweenLineAndCircle(
                                A, B, // the line
                                C, square_radius, // circle center and square radius (2^2)
                                &inter1, &inter2
                ) == 1
        );
        // vertical lines are ok !


        // only one intersection between Cirle and segment [C;inter1]
        assert(geometry::intersectionsBetweenSegmentAndCircle(
                                C, inter1, // segment [circle center; intersection of Circle and (AB)]
                                C, square_radius,
                                &inter1, &inter2
                ) == 1
        );
        // behinder is inside the circle
        Coordinates behinder = geometry::behindIntersectionOfLines(
                        C, inter1, // the line where behinder will be
                        A, B       // behinder will be behind this line
        );
        assert(geometry::pointInsideCircle(behinder, C, radius));
        // no intersection between Cirle and segment [C;behinder]
        assert(geometry::intersectionsBetweenSegmentAndCircle(
                                C, behinder, // segment [circle center; behinder]
                                C, square_radius,
                                &inter2, &inter2
                ) == 0
        );
        // behinder computing is ok !
}










/***************************************************
 * SQUARE DISTANCE BETWEEN POINTS
 ***************************************************/
/**
 * @param x1 coordinate in x for first point
 * @param y1 coordinate in y for first point
 * @param x2 coordinate in x for second point
 * @param y2 coordinate in y for second point
 * @return square distance between the two points
 * @warning return value is square of distance, not real distance
 */
float geometry::squareDistanceBetweenPoints(float x1, float y1, float x2, float y2) {
        return (x1-x2) * (x1-x2) + (y1-y2) * (y1-y2);
}
/**
 * @param A Coordinates of a point of triangle
 * @param B Coordinates of a point of triangle
 * @return square distance between the two points
 * @warning return value is square of distance, not real distance
 */
float geometry::squareDistanceBetweenPoints(Coordinates A, Coordinates B) {
        return squareDistanceBetweenPoints(A.x(), A.y(), B.x(), B.y());
}








/***************************************************
 * POINT IN CIRCUMCIRCLE OF
 ***************************************************/
/**
 * @param p1 Coordinates of a point of triangle
 * @param p2 Coordinates of a point of triangle
 * @param p3 Coordinates of a point of triangle
 * @param p0 Coordinates of tested point
 * @return true iff tested point is in circumcircle of triangle composed by p1, p2 and p3.
 * @note false iff tested point is exactly on the circle
 * @note false iff circle defined by three aligned points
 * @note assume that Y axis IS inverted
 */
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
        // NB: the algorithm assume that Y axis is inverted.
        // determinant: AEI + BFG + CDH - AFH - BDI - CEG
        // NB: circumcircle points (p1,p2,p3) must be in clockwise order. Else, determinant sign must be changed.

        //if(geometry::pointInCounterClockwiseOrder((unsigned int)3, p1, p2, p3)) {
                //logs("%u:HEAVYDEBUG: (%f;%f) (%f;%f) (%f;%f)\n", __LINE__,
                        //p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y()
                    //);
                //assert(geometry::pointInCounterClockwiseOrder(3, p1, p2, p3));
        //}
        // abnormal case : aligned points : p0 is not in the circumcircle
        if(geometry::alignedPoints(p1, p2, p3)) {
                assert(geometry::signedPolygonArea(3, p1, p2, p3) == 0.);
                return false;
        }

        // according to algorithm definition, determinant sign must be changed if points not in clockwise order
        float d = (geometry::pointInClockwiseOrder(3, p1, p2, p3) ? 1. : -1.)*(
        //float d = (
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
        assert(fabs(d) > -1); // According to Murphy's law, it will happen.
#endif
        //std::cout << "points: {" << p1 << ", " << p2 << ", " << p3 << "} are in "
                  //<< (geometry::pointInClockwiseOrder(3, p1, p2, p3) ? "" : "counter ")
                  //<< "clockwise order; tested: " << p0
                  //<< "; d = " << d << std::endl;
        return d > 0.;
}








/***************************************************
 * CIRCUMCENTER OF
 ***************************************************/
/**
 * @param A Coordinates of a point of triangle
 * @param B Coordinates of a point of triangle
 * @param C Coordinates of a point of triangle
 * @return C Coordinates of triangle's circumcenter
 */
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
/**
 * @param x1 x coordinate of point 1
 * @param y1 y coordinate of point 1
 * @param x2 x coordinate of point 2
 * @param y2 y coordinate of point 2
 * @param x x coordinate of tested point
 * @param y y coordinate of tested point
 * @return result of dot product for given points
 * @see pointInTriangle function that does an important use of this function
 */
inline float collideAt_side(float x1, float y1, float x2, float y2, float x, float y) {
         return (y2 - y1)*(x - x1) + (-x2 + x1)*(y - y1);
}

/**
 * @param x1 x coordinate of point 1
 * @param y1 y coordinate of point 1
 * @param x2 x coordinate of point 2
 * @param y2 y coordinate of point 2
 * @param x x coordinate of tested point
 * @param y y coordinate of tested point
 * @return square distance between point (x,y) and segment [(x1,y1)(x2,y2)]
 * @see pointInTriangle function that does an important use of this function
 */
inline float collideAt_squareDistancePointToSegment(float x1, float y1, float x2, float y2, float x, float y) {
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
 * @param p1 Coordinates of a point of triangle
 * @param p2 Coordinates of a point of triangle
 * @param p3 Coordinates of a point of triangle
 * @param p0 Coordinates of tested point
 * @return true iff tested point is in triangle composed by p1, p2 and p3.
 * @note assume that p1, p2 and p3 are given in counterclockwise order
 */
bool geometry::pointInTriangle(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0) {
#ifdef DEBUG
        std::vector<Coordinates*> assert_data = {&p1, &p2, &p3};
        assert(geometry::pointInCounterClockwiseOrder(assert_data));
#endif
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
                        collide  = collideAt_squareDistancePointToSegment(x1, y1, x2, y2, x, y) <= EPSILON;
                        collide |= collideAt_squareDistancePointToSegment(x2, y2, x3, y3, x, y) <= EPSILON;
                        collide |= collideAt_squareDistancePointToSegment(x3, y3, x1, y1, x, y) <= EPSILON;
                }
        }
        return collide;
}








/***************************************************
 * SQUARE DISTANCE BETWEEN SEGMENT AND POINT
 ***************************************************/
/**
 * @param x1 coordinate in x for first point of segment
 * @param y1 coordinate in y for first point of segment
 * @param x2 coordinate in x for second point of segment
 * @param y2 coordinate in y for second point of segment
 * @param px coordinate in x of tested point
 * @param py coordinate in y of tested point
 * @return shorter square distance between segment and point
 * @warning return value is square of distance, not real distance
 */
float geometry::squareDistanceBetweenSegmentAndPoint(float x1, float y1, float x2, float y2, float px, float py) {
        float distance = -1.;
        // Algorithm found on
        //   http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
        const float square_len = geometry::squareDistanceBetweenPoints(x1, y1, x2, y2);  // distance squared
        if(abs(square_len) < EPSILON)
                distance = geometry::squareDistanceBetweenPoints(x1, y1, px, py);

        Coordinates pxy1(px-x1, py-y1), pxy2(px-x2, py-y2);
        const float t = (pxy1.x()*pxy2.x() + pxy1.y()*pxy2.y()) / square_len;

        if(t < 0.) {
                distance = geometry::squareDistanceBetweenPoints(px, py, x1, y1);
        } else if(t > 1.) {
                distance = geometry::squareDistanceBetweenPoints(px, py, x2, y2);
        } else {
                distance = geometry::squareDistanceBetweenPoints(px, py, x1 + t*(x2-x1), y1 + t*(y2-y1));
        }

        return distance;
}









/***************************************************
 * PARALLELS LINES
 ***************************************************/
/**
 * @param A Coordinates of a line point
 * @param B Coordinates of another line point
 * @param C Coordinates of a line point
 * @param D Coordinates of another line point
 * @return true if lines are parallels
 * @warning if A == B or C == D, lines are parallel
 */
bool geometry::parallelLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        // source: https://en.wikipedia.org/wiki/Line-line_intersection#Mathematics
        //if(A == B)
                //logs("A(%f;%f) == B(%f;%f)\n", A.x(), A.y(), B.x(), B.y());
        //if(C == D)
                //logs("C(%f;%f) == D(%f;%f)\n", C.x(), C.y(), D.x(), D.y());
        return fabs((A.x()-B.x()) * (C.y()-D.y())  -  (A.y()-B.y()) * (C.x()-D.x())) < EPSILON;
}





/***************************************************
 * ALIGNED POINTS
 ***************************************************/
/**
 * @param A Coordinates of a point
 * @param B Coordinates of another point
 * @param C Coordinates of another point
 * @return true if points are aligned
 * @note prediction performed with a call to geometry::parallelLines(4)
 */
bool geometry::alignedPoints(Coordinates A, Coordinates B, Coordinates C) {
        return parallelLines(A, B, A, C);
}






/***************************************************
 * INTERSECTION OF LINES
 ***************************************************/
/**
 * @param A Coordinates of a line point
 * @param B Coordinates of another line point
 * @param C Coordinates of a line point
 * @param D Coordinates of another line point
 * @return Coordinates of intersection of the lines
 * @assert lines are not parallel
 */
Coordinates geometry::intersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        assert(!geometry::parallelLines(A,B,C,D));
        Coordinates intersec;
        float det = (A.x()-B.x()) * (C.y()-D.y())  -  (A.y()-B.y()) * (C.x()-D.x());
        float fA = A.x()*B.y() - A.y()*B.x();
        float fB = C.x()*D.y() - C.y()*D.x();
        intersec.setX((fA*(C.x()-D.x()) - (A.x()-B.x())*fB) / det);
        intersec.setY((fA*(C.y()-D.y()) - (A.y()-B.y())*fB) / det);
        return intersec;
}






/***************************************************
 * BEHIND INTERSECTION OF LINES
 ***************************************************/
/**
 * @param A Coordinates of a line point
 * @param B Coordinates of another line point
 * @param C Coordinates of a line point
 * @param D Coordinates of another line point
 * @return Coordinates of a point that is behind the intersection of the lines, and on (A;B)
 * @note distance between returned point and intersection of lines is equal to EPSILON
 * @note returned point is nearer of A than B; difference is about EPSILON
 * @assert lines are not parallel
 */
Coordinates geometry::behindIntersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        Coordinates intersec = intersectionOfLines(A, B, C, D), behinder(0, 0);
        // behinder is the intersection of the segment [AB] and circle of center A and
        //      radius equal to |AB| - EPSILON
        float circle_radius = A.distanceTo(intersec) - EPSILON*1;
        intersectionsBetweenSegmentAndCircle(
                        A, B, // segment
                        A, circle_radius*circle_radius, // circle center and radius
                        &behinder, &intersec // solution in behinder, and intersec is like a /dev/null
        );
        return behinder;
}








/***************************************************
 * COLLISION BETWEEN SEGMENT AND LINE
 ***************************************************/
/**
 * @param O Coordinates of a segment point
 * @param P Coordinates of another segment point
 * @param A Coordinates of a line point
 * @param B Coordinates of another line point
 * @return true if [OP] segment cross with (AB) line
 * @return 0 if [OP] and (AB) don't cross, 1 if ]OP[ cross (AB) or 2 if O or P are on (AB)
 */
bool geometry::collisionBetweenSegmentAndLine(Coordinates O, Coordinates P, Coordinates A, Coordinates B) {
        // 0,1    1,0   0,0     2,2
        float AB_x = B.x() - A.x();  // 2
        float AB_y = B.y() - A.y();  // 2
        float AP_x = P.x() - A.x();  // 1
        float AP_y = P.y() - A.y();  // 0
        float AO_x = O.x() - A.x();  // 0
        float AO_y = O.y() - A.y();  // 1
        // -2 * 2
        float tmp = (AB_x*AP_y - AB_y*AP_x)*(AB_x*AO_y - AB_y*AO_x);
        if(fabs(tmp - 0.) < EPSILON)    tmp = 2;
        else if(tmp < 0.)               tmp = 1;
        else                            tmp = 0;
        return tmp;
}







/***************************************************
 * COLLISION BETWEEN SEGMENT AND SEGMENT
 ***************************************************/
/**
 * @param A Coordinates of a first segment point
 * @param B Coordinates of another first segment point
 * @param C Coordinates of a second segment point
 * @param D Coordinates of another second segment point
 * @return true if [AB] segment cross with [CD] segment
 */
bool geometry::collisionBetweenSegmentAndSegment(Coordinates A, Coordinates B, Coordinates C, Coordinates D) {
        return collisionBetweenSegmentAndLine(A, B, C, D) && collisionBetweenSegmentAndLine(C, D, A, B);
}








/***************************************************
 * COLLISION BETWEEN LINE AND CIRCLE
 ***************************************************/
/**
 * @param A Coordinates of a line point
 * @param B Coordinates of another line point
 * @param C Coordinates of circle's center
 * @param radius of the circle
 * @return true if circle collide with (AB) line
 */
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
 * POINT INSIDE CIRCLE
 ***************************************************/
/**
 * @param A Coordinates of tested point
 * @param C Coordinates of circle's center
 * @param radius of the circle
 * @return true if tested point is inside circle
 * @note return false if point is exactly on circle
 */
bool geometry::pointInsideCircle(const Coordinates A, const Coordinates C, const float radius) {
        return (A.x()-C.x())*(A.x()-C.x()) + (A.y()-C.y())*(A.y()-C.y()) < radius*radius;
}








/***************************************************
 * POINT ON CIRCLE
 ***************************************************/
/**
 * @param A Coordinates of tested point
 * @param C Coordinates of circle's center
 * @param radius of the circle
 * @return true if tested point is on circle, false if tested point is inside or outside the circle
 */
bool geometry::pointOnCircle(const Coordinates A, const Coordinates C, const float radius) {
        return fabs((A.x()-C.x())*(A.x()-C.x()) + (A.y()-C.y())*(A.y()-C.y()) - radius*radius) < EPSILON;
}








/***************************************************
 * SIGNED POLYGON AREA
 ***************************************************/
/**
 * @param n the numbers of elements in variadic parameter
 * @param args a va_list operationnal on wanted n elements
 * @return area of given polygon, signed. (negativ if counterclockwise)
 * @note consider that Y axis is NOT inverted. Else, sign of returned value is false
 * @see pointInClockwiseOrder and pointInCounterClockwiseOrder functions.
 */
float geometry::signedPolygonArea(const unsigned int n, ...) {
        //std::cout<<__LINE__<<":signedPolygonArea(n, ...): " << n << std::endl;
        std::vector<Coordinates*> coords;
        va_list args;
        va_start(args, n);
        for(unsigned int j = 0; j < n; j++) {
            Coordinates x = va_arg(args, Coordinates);
            coords.push_back(&x);
        }
        va_end(args);
        return  geometry::signedPolygonArea(coords);

}


/**
 * @param coords vector of Coordinates that will be tested
 * @return area of given polygon, signed. (negativ if counterclockwise)
 * @note consider that Y axis is NOT inverted. Else, sign of returned value is false
 * @see pointInClockwiseOrder and pointInCounterClockwiseOrder functions.
 */
float geometry::signedPolygonArea(const std::vector<Coordinates> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        float sum = 0.;
        //std::cout<<__LINE__<<":geometry::signedPolygonArea(const std::vector<Coordinates> coords): { ";
        if(coords.size() > 1) {
                unsigned int summed = 1;
                for(; summed < coords.size(); summed++) {
                        sum += (coords[summed].x() - coords[summed-1].x())
                                * (coords[summed].y() + coords[summed-1].y());
                }
                summed--; // target is the last one
                sum += (coords[0].x()-coords[summed].x())
                        * (coords[0].y()+coords[summed].y());
        }
        //for(auto i : coords) std::cout << i << ", ";
        //std::cout << "};\t=>\t";
        //std::cout << "sum/2. = " << sum / 2. << "\n";
        return sum / 2.; // if negativ, its counter-clockwise.
}


/**
 * @param coords vector of address of Coordinates that will be tested
 * @return area of given polygon, signed. (negativ if counterclockwise)
 * @note consider that Y axis is NOT inverted. Else, sign of returned value is false
 * @see pointInClockwiseOrder and pointInCounterClockwiseOrder functions.
 */
float geometry::signedPolygonArea(const std::vector<Coordinates*> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        float sum = 0.;
        //std::cout<<__LINE__<<":geometry::signedPolygonArea(const std::vector<Coordinates*> coords): { ";
        if(coords.size() > 1) {
                unsigned int summed = 1;
                for(; summed < coords.size(); summed++) {
                        sum += (coords[summed]->x() - coords[summed-1]->x())
                                * (coords[summed]->y() + coords[summed-1]->y());
                }
                summed--; // target is the last one
                sum += (coords[0]->x()-coords[summed]->x())
                        * (coords[0]->y()+coords[summed]->y());
        }
        //for(auto i : coords) std::cout << *i << ", ";
        //std::cout << "};\t=>\t";
        //std::cout << "sum/2. = " << sum / 2. << "\n";
        return sum / 2.; // if negativ, its counter-clockwise.
}






/***************************************************
 * (COUNTER) CLOCKWISE SIGNED POLYGON AREA
 ***************************************************/
/**
 * @param signed_area the signed area of any polygon
 * @return true iff given signed area is 0 or a signed area of a clockwise ordered polygon
 * @note consider that Y axis IS "inverted". Else, returned value is false
 * @note as geometry module assume that Y axis IS inverted, positiv area returns false
 * @see http://stackoverflow.com/a/1165943/3077939
 */
bool geometry::clockwiseSignedPolygonArea(float signed_area) {
        return signed_area <= 0.;
}
/**
 * @param signed_area the signed area of any polygon
 * @return true iff given signed area is 0 or an impossible area for a clockwise ordered polygon
 * @note consider that Y axis IS "inverted". Else, returned value is false
 */
bool geometry::counterclockwiseSignedPolygonArea(float signed_area) {
        if(abs(signed_area) < EPSILON)
                return true;
        else    return not geometry::clockwiseSignedPolygonArea(signed_area);
}







/***************************************************
 * POINT IN COUNTER CLOCKWISE ORDER
 ***************************************************/
/**
 * @param n the number of Coordinates sended
 * @param ... Coordinates objects adresses (pointers to Coordinates)
 * @return true iff n <= 1 or received Coordinates are given in Counter Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInCounterClockwiseOrder(const unsigned int n, ...) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        //std::cout<<__LINE__<<":pointInCounterClockwiseOrder(n, ...): " << n << std::endl;
        // get a vector from variadic arguments and call overloaded function
        std::vector<Coordinates> coords;
        va_list args;
        va_start(args, n);
        for(unsigned int j = 0; j < n; j++) {
            coords.push_back(va_arg(args, Coordinates));
        }
        va_end(args);
        return geometry::counterclockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}



/**
 * @param coords Coordinates that will be tested
 * @return true iff Coordinates are stored in Counter Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInCounterClockwiseOrder(const std::vector<Coordinates*> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        // if negativ, its counter-clockwise.
        //std::cout<<__LINE__<<":pointInCounterClockwiseOrder(vector<Coordinates*>): " << coords.size() << std::endl;
        return geometry::counterclockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}



/**
 * @param coords Coordinates that will be tested
 * @return true iff Coordinates are stored in Counter Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInCounterClockwiseOrder(const std::vector<Coordinates> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        // if negativ, its counter-clockwise.
        //std::cout<<__LINE__<<":pointInCounterClockwiseOrder(vector<Coordinates>): " << coords.size() << std::endl;
        return geometry::counterclockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}




/***************************************************
 * POINT IN CLOCKWISE ORDER
 ***************************************************/
/**
 * @param n the number of Coordinates sended
 * @param ... Coordinates
 * @return true iff n == 0 or received Coordinates are given in Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInClockwiseOrder(const unsigned int n, ...) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        //std::cout<<__LINE__<<":pointInClockwiseOrder(n, ...): " << n << std::endl;
        // get a vector from variadic arguments and call overloaded function
        std::vector<Coordinates> coords;
        va_list args;
        va_start(args, n);
        for(unsigned int j = 0; j < n; j++) {
            coords.push_back(va_arg(args, Coordinates));
        }
        va_end(args);
        return geometry::clockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}


/**
 * @param coords Coordinates that will be tested
 * @return true iff Coordinates are stored in Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInClockwiseOrder(const std::vector<Coordinates*> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        // if negativ, its counter-clockwise.
        //std::cout<<__LINE__<<":pointInClockwiseOrder(vector<Coordinates*>): " << coords.size() << std::endl;
        return geometry::clockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}


/**
 * @param coords Coordinates that will be tested
 * @return true iff Coordinates are stored in Clockwise order
 * @note consider that Y axis is NOT inverted. Else, returned value is false
 */
bool geometry::pointInClockwiseOrder(const std::vector<Coordinates> coords) {
        // Solution found here: http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order?rq=1
        // if negativ, its counter-clockwise.
        //std::cout<<__LINE__<<":pointInClockwiseOrder(vector<Coordinates>): " << coords.size() << std::endl;
        return geometry::clockwiseSignedPolygonArea(geometry::signedPolygonArea(coords));
}








/***************************************************
 * COLLISION BETWEEN SEGMENT AND CIRCLE
 ***************************************************/
/**
 * @param A Coordinates of segment point
 * @param B Coordinates of segment point
 * @param C Coordinates of circle's center
 * @param radius of the circle
 * @return true if circle collide with [AB] segment
 */
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
                          || geometry::pointInsideCircle(A, C, radius)
                          || geometry::pointInsideCircle(B, C, radius);
        }
        return collision;
}








/***************************************************
 * PROJECTION OF POINT ON LINE
 ***************************************************/
/**
 * @param A a point on the line
 * @param B another point on the line
 * @param C the evaluated point
 * @return the projection of evaluated point on the line
 */
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
/**
 * @param A Coordinates of a point on the line
 * @param B Coordinates of another point on the line
 * @param C Coordinates of center of circle
 * @param square_radius of the circle
 * @param S1 reference to Coordinates of the first find point of intersection between cricle and line
 * @param S2 reference to Coordinates of the second find point
 * @return number of solution, 0, 1 or 2
 * @note if return 0, S1 and S2 are not modified, if 1, S1 only is modifed, else, both modified
 */
unsigned int geometry::intersectionsBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C,
                float square_radius, Coordinates* S1, Coordinates* S2) {
#if DEBUG
        assert(S1 != NULL);
        assert(S2 != NULL);
#endif
/* EQUATIONS *//*
                r^2  =  (px - cx)^2 + (py - cy)^2
                with P(px;py) and C(cx;cy) as P a point on the circle of center C and radius R.

                we want to know coordinates (i1x;i1y) and (i2x;i2y) of intersections between
                circle and a (AB) line, with A(ax;ay) and B(bx;by).

                First case : line is defined by y = coef*x + ordn
                intersections coordinates can be found by:
                      R^2 = (px - cx)^2 + (py - cy)^2

                      replacing py by line equation:
                      R^2 = (px - cx)^2 + (coef*px + ordn - cy)^2

                      developped:
                      (1 + coeff^2) * px^2
                      + (-2*cx + 2*coef*ordn - 2*coef*cy) * px
                      + -R^2 + cx^2 + cy^2 + ordn^2 - 2*ordn*cy

                      Resolve this second degre equation lead to coordinates of intersections.

                Second case : line is vertical and defined by x = ordn
                intersections coordinates can be found by:
                      R^2 = (px - cx)^2 + (py - cy)^2

                      replacing py by line equation:
                      R^2 = (ordn - cx)^2 + (py - cy)^2

                      developped:
                      py^2
                      + (-2*cy) * py
                      + ordn^2 - 2*ordn*cx + cx^2 + cy^2 - R^2

                      Resolve this second degre equation lead to coordinates of intersections.
                      …


                      But its false, so we admit this solution :
                      y = cy +- sqrt(R^2 - (K - cx)^2)


*/
        float coef, ordn;
        double delta;
        unsigned int nb_solution = 0;
        if(coeffAndOrdnOfLine(A, B, &coef, &ordn)) {
                // line is not vertical
                double cA = 1 + coef*coef;
                double cB = -2*C.x() + 2*coef*ordn - 2*coef*C.y();
                double cC = -square_radius + C.x()*C.x() + ordn*ordn + C.y()*C.y() - 2*ordn*C.y();
                double sqrt_delta = sqrt(cB*cB - 4*cA*cC); // sqrt(bb - 4ac)

                if(sqrt_delta >= 0) {
                        S1->setX(((-cB + sqrt_delta) / (2.*cA)));
                        S1->setY(S1->x() * coef + ordn);
                        nb_solution++;
                }
                if(sqrt_delta > 0) {
                        S2->setX(((-cB - sqrt_delta) / (2.*cA)));
                        S2->setY(S2->x() * coef + ordn);
                        nb_solution++;
                }
        } else {
                // line is vertical
                //float cB = -2*C.y();
                //float cC = -square_radius - 2*ordn*C.x() + C.x()*C.x() + C.y()*C.y();
                //delta = cB*cB - 4*cC;
                delta = sqrt(square_radius - (ordn - C.x())*(ordn - C.x()));

                if(delta >= 0) { // at least one solution
                        S1->setX(ordn);
                        S1->setY(C.y() + delta);
                        nb_solution++;
                }
                if(delta > 0) {  // second solution
                        S2->setX(ordn);
                        S2->setY(C.y() - delta);
                        nb_solution++;
                }
        }
        return nb_solution;
}








/***************************************************
 * INTERSECTIONS BETWEEN SEGMENT AND CIRCLE
 ***************************************************/
/**
 * @param A Coordinates of extremity of [AB]
 * @param B Coordinates of extremity of [AB]
 * @param C Coordinates of center of circle
 * @param square_radius of the circle
 * @param S1 reference to Coordinates of the first find point of intersection between cricle and [AB]
 * @param S2 reference to Coordinates of the second find point
 * @return number of solution, 0, 1 or 2
 * @note if return 2, S1 and S2 are modified, if 1, S1 is modified and maybe S2, else both can be modified
 */
unsigned int geometry::intersectionsBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C,
                float square_radius, Coordinates* S1, Coordinates* S2) {
        unsigned int nb_solution = geometry::intersectionsBetweenLineAndCircle(A, B, C, square_radius, S1, S2);
        float distance_AB = A.squareDistanceTo(B);

        // RETRACT solutions that aren't on [AB]
        if(nb_solution == 2) {
                if(S2->squareDistanceTo(A) > distance_AB || S2->squareDistanceTo(B) > distance_AB) {
                        nb_solution--;
                }
                if(S1->squareDistanceTo(A) > distance_AB || S1->squareDistanceTo(B) > distance_AB) {
                        nb_solution--;
                        *S1 = *S2;
                }
        }
        if(nb_solution == 1) {
                if(S1->squareDistanceTo(A) > distance_AB || S1->squareDistanceTo(B) > distance_AB) {
                        nb_solution--;
                        *S1 = *S2;
                }
        }
        // END
        return nb_solution;
}








/***************************************************
 * COEF AND ORDN OF LINE
 ***************************************************/
/**
 * @param A Coordinates of a point on the line
 * @param B Coordinates of another point on the line
 * @param coef ref of float, modified
 * @param ordn ref of float, modified
 * @return false if line is vertical, else true
 * @warning if line is vertical, ordn only is modifed and represent the offset in x axis
 */
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





