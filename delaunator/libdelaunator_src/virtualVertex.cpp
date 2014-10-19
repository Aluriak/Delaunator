#include "virtualVertex.h"
#include "edge.h"

unsigned int VirtualVertex::last_id = 1;


/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * @return a new and well initialized VirtualVertex
 */
VirtualVertex::VirtualVertex(Vertex* vtx) {
        this->_id = this->last_id++;
        this->ref_vertex = vtx;
}

/**
 * Frees.
 */
VirtualVertex::~VirtualVertex() {
        // Just in case that is not done :
        if(this->ref_vertex != NULL) {
                this->ref_vertex->forget(this);
        }
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/





/***************************************************
 * PREDICATS
 ***************************************************/
/**
 * @return true iff another VirtualVertex is attach to this vertex
 */
bool VirtualVertex::confoundedWithAnotherObject() const {
        return this->vertex()->getObjectCount() > 1;
}






/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * @return Vertex referenced by this
 */
Vertex* VirtualVertex::vertex() const {
        return this->ref_vertex;
}



/**
 * @return Coordinates where this is
 */
Coordinates VirtualVertex::coordinates() const {
        Coordinates c(*this->ref_vertex);
        return c;
}



/**
 * Change place of this by change Vertex reference
 * @param vtx the Vertex that will references this VirtualVertex
 */
void VirtualVertex::setVertex(Vertex* vtx) {
        if(this->ref_vertex != NULL) 
                this->ref_vertex->forget(this);
        this->ref_vertex = vtx;
        if(this->ref_vertex != NULL) 
                this->ref_vertex->take(this);
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/







/***************************************************
 * GRAPH EXPLORATION
 ***************************************************/
/**
 * @return list of VirtualVertex that are directly connected to this instance.
 */
std::list<VirtualVertex*> VirtualVertex::directNeighbors() const {
        std::list<VirtualVertex*> find_nei;
        Edge* edge_cur = this->vertex()->getEdge();
        Edge* edge_ref = edge_cur;
        std::list<VirtualVertex*> l; // container of finded objects
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
 * @return list of VirtualVertex that are at dist_max distance to this instance.
 */
std::list<VirtualVertex*> VirtualVertex::neighborsAt(float dist_max) const {
        std::list<VirtualVertex*> find_nei;
        //TODO
        logs("NEED TO BE IMPLEMENTED: ");
        logs("std::list<VirtualVertex> VirtualVertex::neighborsAt(float dist_max) const\n");
        //TODO
        return find_nei;
}



/**
 * @param nb_nei the number of neighbors
 * @return list of VirtualVertex that are the closer of this. List size is equal to nb_nei at the most.
 */
std::list<VirtualVertex*> VirtualVertex::nearerNeighbors(unsigned int nb_nei) const {
        std::list<VirtualVertex*> find_nei;
        //TODO
        logs("NEED TO BE IMPLEMENTED: ");
        logs("std::list<VirtualVertex*> VirtualVertex::nearerNeighbors(unsigned int nb_nei) const\n");
        //TODO
        return find_nei;
}







