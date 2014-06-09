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



}


#endif
