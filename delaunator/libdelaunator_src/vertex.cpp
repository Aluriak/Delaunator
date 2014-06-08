#include "vertex.h"
#include "edge.h"

unsigned int Vertex::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Creat a new Vertex.
 * @param x coordinate
 * @param y coordinate
 * @param edge reference to an Edge that have this new Vertex for origin
 * @return a new and well initialized Vertex
 */
Vertex::Vertex(const float x, const float y, Edge* edge) : Coordinates(x,y), edge(edge) {
        this->id = this->last_id++;
        if(this->edge != NULL)  this->edge->setOriginVertex(this);
}

/**
 * Creat a new Vertex.
 * @param c Coordinates that defined position of new Vertex
 * @param edge reference to an Edge that have this new Vertex for origin
 * @return a new and well initialized Vertex
 */
Vertex::Vertex(const Coordinates c, Edge* edge) : Coordinates(c), edge(edge) {
        this->id = this->last_id++;
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * @return number of neighbour directly connected by an edge
 */
unsigned int Vertex::neighbourCount() const {
        unsigned int counter = 0;
        Edge* edge = this->edge;
        do {
                counter++;
                edge = edge->rotLeftEdge();
        } while(edge != this->edge);
        return counter;
}





/***************************************************
 * OPERATORS
 ***************************************************/
/**
 * @param othr other Vertex that equality is tested
 * @return true iff given vertex have the same coordinates of this
 */
bool Vertex::operator==(const Vertex& othr) {
        return Coordinates::operator==(othr) && this->edge == othr.edge;
}




/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * @return an Edge that had this Vertex for origin
 */
Edge* Vertex::getEdge() const { return this->edge; }
/**
 * @param e the reference to an Edge, the new Edge that have this Vertex for origin
 */
void Vertex::setEdge(Edge* e) { this->edge = e; }



/***************************************************
 * PRIVATE METHODS
 ***************************************************/



/***************************************************
 * EXTERNAL METHODS
 ***************************************************/
#if !SWIG
std::ostream& operator<<(std::ostream& flux, Coordinates const &c) {
        return flux << "(" << c.x() << ";" << c.y() << ")";
}
std::ostream& operator<<(std::ostream& flux, Vertex const &c) {
        return flux << "(" << c.x() << ";" << c.y() 
                << (c.getEdge() != NULL ? ";linked)" : ";free)");
}
#endif



