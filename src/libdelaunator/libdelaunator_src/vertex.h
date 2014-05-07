#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Edge;




/*
 * VERTEX CLASS DEFINITION
 */
struct Coordinates {
		Coordinates(const float x = 0., const float y = 0.) : x(x), y(y) {}
	// PUBLIC METHODS
                float squareDistanceTo(const Coordinates& othr) {
                        return (this->x - othr.x)*(this->x - othr.x)+(this->y - othr.y)*(this->y - othr.y);
                }
                float distanceTo(const Coordinates& othr) {
                        return sqrt((this->x - othr.x)*(this->x - othr.x)+(this->y - othr.y)*(this->y - othr.y));
                }
        // OPERATORS
                bool operator==(const Coordinates& othr) {
                        return fabs(othr.x - this->x) < EPSILON && fabs(othr.y - this->y) < EPSILON;
                }
	// ATTRIBUTES
                float x, y;
};

struct Vertex : public Coordinates {
        public:
        // CONSTRUCTOR
                Vertex(const float = 0., const float = 0., Edge* = NULL);
                Vertex(const Coordinates c, Edge* = NULL);
        // PUBLIC METHODS
                unsigned int neighbourCount() const;
        // ACCESSORS
                unsigned int getID()  const { return this->id; }
                Edge* getEdge() const;
                void setEdge(Edge* e);
        // OPERATORS
                bool operator==(const Vertex&);
        // ATTRIBUTES
        private:
        // ATTRIBUTES
                Edge* edge; // edge has this Vertex as origin.
                unsigned int id;
                static unsigned int last_id;
};



// EXTERNAL METHODS
#if !SWIG
        std::ostream& operator<<(std::ostream&, Coordinates const &);
        std::ostream& operator<<(std::ostream&, Vertex const &);
#endif


#endif
