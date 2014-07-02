#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "vertex.h"



/*
 * DEFINES
 */





/**
 * Geometry is a namespace that define lots of useful geometry functions.
 */
namespace geometry {


        /**
         * @param p1 Coordinates of a point of triangle
         * @param p2 Coordinates of a point of triangle
         * @param p3 Coordinates of a point of triangle
         * @param p0 Coordinates of tested point
         * @return true iff tested point is in circumcircle of triangle composed by p1, p2 and p3.
         */
        bool pointInCircumcircleOf(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0);


        /**
         * @param A Coordinates of a point of triangle
         * @param B Coordinates of a point of triangle
         * @param C Coordinates of a point of triangle
         * @return C Coordinates of triangle's circumcenter
         */
        Coordinates circumcenterOf(Coordinates A, Coordinates B, Coordinates C);


        /**
         * @param p1 Coordinates of a point of triangle
         * @param p2 Coordinates of a point of triangle
         * @param p3 Coordinates of a point of triangle
         * @param p0 Coordinates of tested point
         * @return true iff tested point is in triangle composed by p1, p2 and p3.
         */
        bool pointInTriangle(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0);


        /**
         * @param x1 coordinate in x for first point of segment
         * @param y1 coordinate in y for first point of segment
         * @param y2 coordinate in y for second point of segment
         * @param x2 coordinate in x for second point of segment
         * @param px coordinate in x of tested point
         * @param py coordinate in y of tested point
         * @return shorter square distance between segment and point
         * @warning return value is square of distance, not real distance
         */
        float distanceBetweenPointAndSegment(float x1, float x2, float y1, float y2, float px, float py);


        /**
         * @param A Coordinates of a line point
         * @param B Coordinates of another line point
         * @param C Coordinates of a line point
         * @param D Coordinates of another line point
         * @return true if lines are parallels
         */
        bool parallelsLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D);


        /**
         * @param A Coordinates of a line point
         * @param B Coordinates of another line point
         * @param C Coordinates of a line point
         * @param D Coordinates of another line point
         * @return Coordinates of intersection of the lines
         * @warning if lines are parallels, result is false
         */
        Coordinates intersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D);


        /**
         * @param O Coordinates of a segment point
         * @param P Coordinates of another segment point
         * @param A Coordinates of a line point
         * @param B Coordinates of another line point
         * @return true if [OP] segment cross with (AB) line
         */
        bool collisionBetweenSegmentAndLine(Coordinates O, Coordinates P, Coordinates A, Coordinates B);


        /**
         * @param A Coordinates of a first segment point
         * @param B Coordinates of another first segment point
         * @param C Coordinates of a second segment point
         * @param D Coordinates of another second segment point
         * @return true if [AB] segment cross with [CD] segment
         */
        bool collisionBetweenSegmentAndSegment(Coordinates A, Coordinates B, Coordinates C, Coordinates D);


        /**
         * @param A Coordinates of a line point
         * @param B Coordinates of another line point
         * @param C Coordinates of circle's center 
         * @param radius of the circle
         * @return true if circle collide with (AB) line
         */
        bool collisionBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius);


        /**
         * @param A Coordinates of tested point
         * @param C Coordinates of circle's center 
         * @param radius of the circle
         * @return true if tested point is in circle
         */
        bool pointInCircle(Coordinates A, Coordinates C, float radius);


        /**
         * @param A Coordinates of segment point
         * @param B Coordinates of segment point
         * @param C Coordinates of circle's center 
         * @param radius of the circle
         * @return true if circle collide with [AB] segment
         */
        bool collisionBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius);


        /**
         * @param A a point on the line
         * @param B another point on the line
         * @param C the evaluated point
         * @return the projection of evaluated point on the line
         */
        Coordinates projectionOfPointOnLine(Coordinates A, Coordinates B, Coordinates C);


        /**
         * @param A Coordinates of a point on the line
         * @param B Coordinates of another point on the line
         * @param C Coordinates of center of circle
         * @param radius of the circle 
         * @param S1 reference to Coordinates of the first find point of intersection between cricle and line
         * @param S2 reference to Coordinates of the second find point
         * @return number of solution, 0, 1 or 2
         * @note if return 0, S1 and S2 are not modified, if 1, S1 only is modifed, else, both modified
         */
        unsigned int intersectionsBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, 
                        float radius, Coordinates* S1, Coordinates* S2);



        /**
         * @param A Coordinates of a point on the line
         * @param B Coordinates of another point on the line
         * @param coef ref of float, modified
         * @param ordn ref of float, modified
         * @return false if line is vertical, else true
         * @warning if line is vertical, ordn only is modifed and represent the offset in x axis
         */
        bool coeffAndOrdnOfLine(Coordinates A, Coordinates B, float* coef, float* ordn);
}


#endif
