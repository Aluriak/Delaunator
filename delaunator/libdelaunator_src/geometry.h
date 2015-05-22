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
 * All functions assume that Y axis IS "inverted" ;
 * in other words, all functions assume that bigger numbers are to the bottom.
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

        bool pointInsideCircle(const Coordinates A, const Coordinates C, const float radius);

        bool pointOnCircle(const Coordinates A, const Coordinates C, const float radius);

        //template<typename ... Ts>
        //float signedPolygonArea(Ts...);
        float signedPolygonArea(const unsigned int, ...);
        float signedPolygonArea(const std::vector<Coordinates>);
        float signedPolygonArea(const std::vector<Coordinates*>);

        bool clockwiseSignedPolygonArea(float);
        bool counterclockwiseSignedPolygonArea(float);

        bool pointInCounterClockwiseOrder(const unsigned int, ...);
        bool pointInCounterClockwiseOrder(const std::vector<Coordinates*>);
        bool pointInCounterClockwiseOrder(const std::vector<Coordinates>);
        bool pointInClockwiseOrder(const unsigned int, ...);
        bool pointInClockwiseOrder(const std::vector<Coordinates*>);
        bool pointInClockwiseOrder(const std::vector<Coordinates>);

        bool collisionBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C, float radius);

        Coordinates projectionOfPointOnLine(Coordinates A, Coordinates B, Coordinates C);

        unsigned int intersectionsBetweenLineAndCircle(Coordinates A, Coordinates B, Coordinates C,
                        float square_radius, Coordinates* S1, Coordinates* S2);

        unsigned int intersectionsBetweenSegmentAndCircle(Coordinates A, Coordinates B, Coordinates C,
                        float square_radius, Coordinates* S1, Coordinates* S2);

        bool coeffAndOrdnOfLine(Coordinates A, Coordinates B, float* coef, float* ordn);
}


#endif
