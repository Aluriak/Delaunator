#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "trianguledObject.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Edge;
class TrianguledObject;




/**
 * Coordinates are composed by two float values, x and y.
 */
struct Coordinates {
        public:
        // CONSTRUCTOR
		Coordinates(const float x = 0., const float y = 0.) {
                        this->setCoord(x, y);
                }
	// PUBLIC METHODS
                float squareDistanceTo(const Coordinates& othr) {
                        return (this->_x - othr._x)*(this->_x - othr._x)+(this->_y - othr._y)*(this->_y - othr._y);
                }
                float distanceTo(const Coordinates& othr) {
                        return sqrt(
                                (this->_x - othr._x)*(this->_x - othr._x)+(this->_y - othr._y)*(this->_y - othr._y)
                                );
                }
        // ACCESSORS
                float x() const { return this->_x; }
                float y() const { return this->_y; }
                void setX(float x) { this->_x = round_float(x, EPSILON*10.); }
                void setY(float y) { this->_y = round_float(y, EPSILON*10.); }
                void setCoord(float x, float y) { this->setX(x); this->setY(y); }
        // OPERATORS
                bool operator==(const Coordinates& othr) {
                        return fabs(othr._x - this->_x) < EPSILON && fabs(othr._y - this->_y) < EPSILON;
                }
                bool operator!=(const Coordinates& othr) {
                        return !(*this == othr);
                }
                Coordinates operator+(const Coordinates& c) {
                        Coordinates r(this->_x + c._x, this->_y + c._y);
                        return r;
                }
                Coordinates operator-(const Coordinates& c) {
                        Coordinates r(this->_x - c._x, this->_y - c._y);
                        return r;
                }
                Coordinates operator+=(const Coordinates& c) {
                        return *this + c;
                }
        private:
	// ATTRIBUTES
                float _x, _y;
};


/**
 * Vertex are Coordinates with a unique ID and that is linked to an Edge.
 */
class Vertex : public Coordinates {
        public:
        // CONSTRUCTOR
                Vertex(const float = 0., const float = 0., Edge* = NULL);
                Vertex(const Coordinates c, Edge* = NULL);
        // PUBLIC METHODS
                unsigned int neighbourCount() const;
                bool isNeighbourOf(Vertex*) const;
                void giveTrianguledObjectsTo(Vertex*);
                void forget(TrianguledObject*);
        // ACCESSORS
                unsigned int getID()  const { return this->id; }
                Edge* getEdge() const;
                void setEdge(Edge* e);
                bool isACorner() const;
        // OPERATORS
                bool operator==(const Vertex&);
        private:
        // ATTRIBUTES
                Edge* edge; // edge has this Vertex as origin.
                unsigned int id;
                static unsigned int last_id;
                std::list<TrianguledObject*> objects;
};



// EXTERNAL METHODS
#if !SWIG
        std::ostream& operator<<(std::ostream&, Coordinates const &);
        std::ostream& operator<<(std::ostream&, Vertex const &);
#endif


#endif
