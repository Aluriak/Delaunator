
/**
 * call that once for verify integrity of geometry fonctions
 * @warning: assertion fail if detected problem
 */
void unit_tests() {
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

