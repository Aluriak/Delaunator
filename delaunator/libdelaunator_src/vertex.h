#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED



/*
 * LIBRARIES
 */
// LOCAL MODULES
#include "commons.h"
#include "virtualVertex.h"



/*
 * DEFINES
 */




/*
 * PREDECLARATIONS
 */
class Edge;
class VirtualVertex;




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
                float squareDistanceTo(const Coordinates& othr) const {
                        return (this->_x - othr._x)*(this->_x - othr._x)+(this->_y - othr._y)*(this->_y - othr._y);
                }
                float distanceTo(const Coordinates& othr) const {
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
                        return (fabs(othr._x - this->_x) < EPSILON) && (fabs(othr._y - this->_y) < EPSILON);
                }
                bool operator!=(const Coordinates& othr) {
                        return not (*this == othr);
                }
                Coordinates operator+(const Coordinates& c) {
                        return Coordinates(this->_x + c._x, this->_y + c._y);
                }
                Coordinates operator-(const Coordinates& c) {
                        return Coordinates(this->_x - c._x, this->_y - c._y);
                }
                Coordinates operator/(const float value) {
                        return Coordinates(this->_x / value, this->_y / value);
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
                ~Vertex();
        // PUBLIC METHODS
                unsigned int neighbourCount() const;
                void take(VirtualVertex*, Vertex* = NULL);
                void giveVirtualVerticesTo(Vertex*);
                void forget(VirtualVertex*);
        // ACCESSORS
                unsigned int getID()            const { return this->id; }
                unsigned int getObjectCount()   const { return this->objects.size(); }
                std::list<VirtualVertex*> getObjects(unsigned int = 0) const;
                VirtualVertex* getFirstObject()        const;
                Edge* getEdge() const;
                void setEdge(Edge* e);
        // PREDICATS
                bool isNeighbourOf(Vertex*) const;
                bool isACorner() const;
                bool have(VirtualVertex*) const;
        private:
        // ATTRIBUTES
                Edge* edge; // edge has this Vertex as origin.
                unsigned int id;
                static unsigned int last_id;
                std::list<VirtualVertex*> objects;
};



#if !SWIG

// EXTERNAL METHODS
        std::ostream& operator<<(std::ostream&, Coordinates const &);
        std::ostream& operator<<(std::ostream&, Vertex const &);



// EXTERNAL TYPES
/**
 * Compare Vertex by distance with reference Coordinates.
 * The nearer of reference is the better.
 */
class VertexComparison {
        public:
	// CONSTRUCTOR
                VertexComparison(const Coordinates c) : reference(c) {}
        // PREDICATS
                bool operator()(const Vertex* left, const Vertex* right) {
                        // STL structures wait generally for the strict weak order (<).
                        // In a case of a priority_queue, its for found the greater object.
                        // But we want to minimize distance to reference, so we use > operator instead of <.
                        return left->squareDistanceTo(this->reference) > right->squareDistanceTo(this->reference);
                }
        private:
	// ATTRIBUTES
                const Coordinates reference;
};

/**
 * Get a hash from a Vertex* value.
 * Used by sets and maps of STL.
 */
struct VertexHash {
          inline size_t operator()(const Vertex* o) const { return (size_t)o; }
};
/**
 * Type of priority queue that compare two Vertex instance, according to their distances to reference Coordinates.
 */
typedef std::priority_queue<const Vertex*, std::vector<Vertex*>, VertexComparison> vertex_comparator;


#endif




#endif
