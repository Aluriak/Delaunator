#include "vertex.h"
#include "edge.h"

unsigned int Vertex::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
Vertex::Vertex(const float x, const float y, Edge* edge) : Coordinates(x,y), edge(edge) {
        this->id = this->last_id++;
        if(this->edge != NULL)  this->edge->setOriginVertex(this);
}

Vertex::Vertex(const Coordinates c, Edge* edge) : Coordinates(c), edge(edge) {
        this->id = this->last_id++;
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * NEIGHBOUR COUNT
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
bool Vertex::operator==(const Vertex& othr) {
        return Coordinates::operator==(othr) && this->edge == othr.edge;
}




/***************************************************
 * ACCESSORS
 ***************************************************/

Edge* Vertex::getEdge() const {
        //if(this->edge != NULL) logs("Get for %u\n", this->edge->getID()); 
        return this->edge; 
}
void Vertex::setEdge(Edge* e) {
        //if(this->edge != NULL && e != NULL) logs("Set for %u over %u\n", e->getID(), this->edge->getID()); 
        this->edge = e; 
}

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



