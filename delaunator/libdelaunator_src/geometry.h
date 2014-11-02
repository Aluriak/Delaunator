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
        void unit_tests(); // call that once for verify integrity of geometry fonctions


        float squareDistanceBetweenPoints(Coordinates A, Coordinates B);
        float squareDistanceBetweenPoints(float x1, float y1, float x2, float y2);

        bool pointInCircumcircleOf(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0);

        Coordinates circumcenterOf(Coordinates A, Coordinates B, Coordinates C);
        
        bool pointInTriangle(Coordinates p1, Coordinates p2, Coordinates p3, Coordinates p0);
        
        float squareDistanceBetweenSegmentAndPoint(float x1, float x2, float y1, float y2, float px, float py);
        
        bool parallelLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D);

        bool alignedPoints(Coordinates A, Coordinates B, Coordinates C);
        
        Coordinates intersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D);

        Coordinates behindIntersectionOfLines(Coordinates A, Coordinates B, Coordinates C, Coordinates D);
        
        bool collisionBetweenSegmentAndLine(Coordinates O, Coordinates P, Coordinates A, Coordinates B);
        
        bool collisionBetweenSegmentAndSegment(Coordinates A, Coordinates B, Coordinates C, Coordinates D);
        
        bool collisionBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius);

        bool pointInsideCircle(Coordinates A, Coordinates C, float radius);

        bool pointOnCircle(Coordinates A, Coordinates C, float radius);
        
        bool collisionBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius);
        
        Coordinates projectionOfPointOnLine(Coordinates A, Coordinates B, Coordinates C);
        
        unsigned int intersectionsBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C, 
                        float square_radius, Coordinates* S1, Coordinates* S2);
        
        unsigned int intersectionsBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C, 
                        float square_radius, Coordinates* S1, Coordinates* S2);
        
        bool coeffAndOrdnOfLine(Coordinates A, Coordinates B, float* coef, float* ordn);
}


#endif
