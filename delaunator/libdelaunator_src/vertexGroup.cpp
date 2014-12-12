#include "vertexGroup.h"
#include "edge.h"



/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Creat a new VertexGroup.
 * @return a new and well initialized VertexGroup
 */
VertexGroup::VertexGroup() : Vertex() {
#ifdef DEBUG
        assert(this->getVertexCount() == 0);
#endif
}



/**
 * Destructor
 * Forget all VirtualVertex
 */
VertexGroup::~VertexGroup() {
        std::list<Vertex*>::iterator it = this->vertices.begin();
        for(; it != this->vertices.end(); it++) {
                this->getObjects().clear();
        }
}




/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/*
 * @return number of neighbour directly connected by an edge
 */
unsigned int VertexGroup::neighbourCount() const {
        unsigned int counter = 0;
        Edge* edge = this->getEdge();
        do {
                counter++;
                edge = edge->rotLeftEdge();
        } while(edge != this->getEdge());
        return counter;
}



/**
 * @param v a VertexGroup that will receive all VirtualVertex of this
 */
void VertexGroup::giveVirtualVerticesTo(Vertex* v) {
        std::list<VirtualVertex*>::iterator it = this->getObjects().begin();
        // assign each object of this to v
        for(; it != this->getObjects().end(); it++) {
                v->take(*it);
        }
        this->getObjects().clear();
}



/**
 * @param trob a VirtualVertex that will be added in list of this
 * @param ancient the Vertex that have trob, and that will forget it, or NULL (default value)
 * @note trob is modified by a call to setVertexGroup(this)
 */
void VertexGroup::take(VirtualVertex* trob, Vertex* ancient) {
        // don't need changes in this case
        if(ancient != this) {
                this->getObjects().push_back(trob);
                trob->setVertex(this);

                if(ancient != NULL) { // forget it !
                        ancient->forget(trob);
                }
        }
}



/**
 * @param trob a VirtualVertex that will be forgeted by this
 * @note if this have not trob in memory, nothing is done by that function.
 * @note trob is not modified
 */
void VertexGroup::forget(VirtualVertex* trob) {
        this->getObjects().remove(trob);
}









/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * Access to all VirtualVertexG of this instance.
 * @param nb number of objects returned. If 0, default or greater than object count, all are returned.
 * @return a std::list of VirtualVertex pointers
 */
std::list<VirtualVertex*> VertexGroup::getObjects(unsigned int nb) const {
        //std::list<VirtualVertex*> l(this->getObjects()); 
        std::list<VirtualVertex*> l; 
        if(nb == 0 || nb >= this->getObjects().size()) {
                nb = this->getObjects().size(); 
        }
        // we will use insert one day
        std::list<VirtualVertex*>::const_iterator it = this->getObjects().begin();
        for(; nb > 0 && it != this->getObjects().end(); it++, nb--) {
                l.push_back(*it);
        }
        return l;
}


/**
 * Access to one VirtualVertex of this instance.
 * @return a VirtualVertex pointer, or NULL if no Object referenced
 */
VirtualVertex* VertexGroup::getFirstObject() const {
        return this->getObjects().front() != NULL ? this->getObjects().front() : NULL;
}









/***************************************************
 * PREDICATS
 ***************************************************/
/**
 * @param v a VertexGroup
 * return true iff an edge have for origin this and for destination v
 */
bool VertexGroup::isNeighbourOf(Vertex* v) const {
        Edge* finder = this->getEdge();
        bool isNei = false;
        do {
                if(finder->destinVertex() == v)
                        isNei = true;
                finder = finder->rotLeftEdge();
        } while(!isNei && finder != this->getEdge());
        return isNei;
}



/**
 * @return true iff this is referenced by triangulation and is one of the four corner vertice
 */
bool VertexGroup::isACorner() const {
        return this->getID() <= 4;
}



/**
 * @param vv a VirtualVertex address
 * @return true iff this have vv in objects list
 */
bool VertexGroup::have(VirtualVertex* vv) const {
        bool bRet = false;
        std::list<VirtualVertex*>::const_iterator it = this->getObjects().begin();
        for(; bRet == false && it != this->getObjects().end(); it++) {
                if((*it) == vv)
                        bRet = true;
        }
        return bRet;
}




