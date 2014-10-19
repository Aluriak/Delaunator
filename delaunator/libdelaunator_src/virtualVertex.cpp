#include "trianguledObject.h"
#include "edge.h"



/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * @return a new and well initialized TrianguledObject
 */
TrianguledObject::TrianguledObject(Vertex* vtx) {
        this->vertex = vtx;
}

/**
 * Frees.
 */
TrianguledObject::~TrianguledObject() {
        // Just in case that is not done :
        if(this->vertex != NULL) {
                this->vertex->forget(this);
        }
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/





/***************************************************
 * PREDICATS
 ***************************************************/
/**
 * @return true iff another TrianguledObject is attach to this vertex
 */
bool TrianguledObject::confoundedWithAnotherObject() const {
        return this->getVertex()->getObjectCount() > 1;
}






/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * @return Vertex referenced by this
 */
Vertex* TrianguledObject::getVertex() const {
        return this->vertex;
}



/**
 * @return Coordinates where this is
 */
Coordinates TrianguledObject::getCoordinates() const {
        Coordinates c(*this->vertex);
        return c;
}



/**
 * @return list of TrianguledObject that are directly connected to this instance.
 */
std::list<TrianguledObject*> TrianguledObject::directNeighbors() const {
        std::list<TrianguledObject*> find_nei;
        Edge* edge_cur = this->getVertex()->getEdge();
        Edge* edge_ref = edge_cur;
        std::list<TrianguledObject*> l; // container of finded objects
        // walk to directs neighbors
        do {
                l = edge_cur->destinVertex()->getObjects();
                find_nei.insert(find_nei.end(), l.begin(), l.end());
                edge_cur = edge_cur->rotLeftEdge();
        } while(edge_cur != edge_ref);
        return find_nei;
}



/**
 * @param dist_max the maximum distance 
 * @return list of TrianguledObject that are at dist_max distance to this instance.
 */
std::list<TrianguledObject*> TrianguledObject::neighborsAt(float dist_max) const {
        std::list<TrianguledObject*> find_nei;
        //TODO
        logs("NEED TO BE IMPLEMENTED: ");
        logs("std::list<TrianguledObject> TrianguledObject::neighborsAt(float dist_max) const\n");
        //TODO
        return find_nei;
}



/**
 * Change place of this by change Vertex reference
 * @param vtx the Vertex that will references this TrianguledObject
 */
void TrianguledObject::setVertex(Vertex* vtx) {
        if(this->vertex != NULL) 
                this->vertex->forget(this);
        this->vertex = vtx;
        if(this->vertex != NULL) 
                this->vertex->take(this);
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/







