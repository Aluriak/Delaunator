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
 * Change place of this by change Vertex reference
 * @param vtx the Vertex that will references this VirtualVertex
 */
void VirtualVertex::setVertex(Vertex* vtx) {
        this->ref_vertex = vtx;
}



/**
 * @return Coordinates where this is
 */
Coordinates VirtualVertex::coordinates() const {
        return Coordinates(*this->vertex());
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/







/***************************************************
 * GRAPH EXPLORATION
 ***************************************************/
/**
 * @return list of VirtualVertex that share the same Vertex.
 */
std::list<VirtualVertex*> VirtualVertex::confundedNeighbors() const {
        std::list<VirtualVertex*> find_nei(this->ref_vertex->getObjects());
        find_nei.erase(std::find(find_nei.begin(), find_nei.end(), this));
        return find_nei;
}



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
 * @param distance_max the maximum distance 
 * @param distance_min the minimum distance. Optionnal. Default, Zero or less mean no minimum.
 * @return list of VirtualVertex that are at dist_max and dist_min included distances to this instance. 
 * @note VirtualVertex are returned in distance increasing order. (nearer first)
 * @note confounded VirtualVertex are not sorted in any particular order
 */
std::list<VirtualVertex*> VirtualVertex::neighborsAt(const float distance_max, const float distance_min) const {
// INITIALIZING
        std::list<VirtualVertex*> find_nei(this->confundedNeighbors());
        vertex_comparator vertices(VertexComparison(*this->ref_vertex));
        std::unordered_set<Vertex*, VertexHash> walked;
        float dist_cur;
        const float dist_max = distance_max*distance_max; // work with square distances
        const float dist_min = distance_min*distance_min;

        // first element
        vertices.push(this->ref_vertex);
        walked.insert(this->ref_vertex);

// TREATMENTS
        while(not vertices.empty()) {
                Vertex* vertex = vertices.top(); // get nearer Vertex
                vertices.pop(); // delete nearer Vertex of vertices exploration
                // treatment on vertex, only if not too far
                dist_cur = vertex->squareDistanceTo(*this->ref_vertex);
                if(dist_cur <= dist_max) {
                        // we will walk this one later
                        // if its also under low limit
                        if(dist_cur >= dist_min) {
                                // add Virtual Vertices to list of finded neighbors
                                find_nei.splice(find_nei.end(), vertex->getObjects());
                        }

                        // for each edge
                        Edge* edge_cur = vertex->getEdge();
                        Edge* edge_ref = edge_cur;
                        do {
                                Vertex* nei = edge_cur->destinVertex();
                                // if not already walked 
                                if(not walked.count(nei)) {
                                        walked.insert(nei);     // now its marked
                                        vertices.push(nei);  // and will be explored
                                }

                                // go next neighbour
                                edge_cur = edge_cur->rotLeftEdge();
                        } while(edge_ref != edge_cur);
                }
                // redo for nearer Vertex !
        }

// ENDING
        walked.clear();
        // remove this from returned list: don't work because call the destructor on this…
        //find_nei.erase(std::find(find_nei.begin(), find_nei.end(), this));
#ifdef DEBUG
        // verification: distance to this must grow bigger
        // because list is sorted
        std::list<VirtualVertex*>::const_iterator it = find_nei.begin();
        float dist_ctrl = -1; 
        for(; it != find_nei.end(); it++) {
                assert(dist_ctrl <= (*it)->vertex()->squareDistanceTo(*this->vertex()));
                dist_ctrl = (*it)->vertex()->squareDistanceTo(*this->vertex());
        }
#endif
        return find_nei;
}



/**
 * @param nb_nei the number of neighbors
 * @return list of VirtualVertex that are the closer of this. List size is equal to nb_nei at the most.
 * @note VirtualVertex are returned in distance increasing order. (nearer first)
 * @note VirtualVertex at the same distance are not sorted in any particular order
 */
std::list<VirtualVertex*> VirtualVertex::nearerNeighbors(const unsigned int nb_nei) const {
        std::list<VirtualVertex*> find_nei;
        vertex_comparator vertices(VertexComparison(*this->ref_vertex));
        std::unordered_set<Vertex*, VertexHash> walked;
        unsigned int remain_nei = nb_nei+1; // when 0 reached, no walk needed anymore; +1 because this don't count
        unsigned int nb_vv;

        // first element
        vertices.push(this->ref_vertex);
        walked.insert(this->ref_vertex);

// TREATMENTS
        while(remain_nei > 0 && not vertices.empty()) {
                Vertex* vertex = vertices.top(); // get nearer Vertex
                vertices.pop(); // delete nearer Vertex of vertices exploration

                // add Virtual Vertices to list of finded neighbors
                nb_vv = vertex->getObjectCount();
                if(nb_vv < remain_nei) { 
                        find_nei.splice(find_nei.end(), vertex->getObjects());
                        remain_nei -= nb_vv;
                } else { // not enough objects
                        find_nei.splice(find_nei.end(), vertex->getObjects(remain_nei));
                        remain_nei = 0;
                }

                // for each edge
                Edge* edge_cur = vertex->getEdge();
                Edge* edge_ref = edge_cur;
                do {
                        Vertex* nei = edge_cur->destinVertex();
                        // if not already walked and not a corner
                        if(not walked.count(nei) and not nei->isACorner()) {
                                walked.insert(nei); // now its marked
                                // we will walk this one later
                                vertices.push(nei);
                        }

                        // go next neighbour
                        edge_cur = edge_cur->rotLeftEdge();
                } while(edge_ref != edge_cur);
                // redo for nearer Vertex !
        }

// ENDING
        walked.clear();
#ifdef DEBUG
        // verification: distance to this must grow bigger
        // because list is sorted
        std::list<VirtualVertex*>::const_iterator it = find_nei.begin();
        float dist_ctrl = -1; 
        for(; it != find_nei.end(); it++) {
                assert(dist_ctrl <= (*it)->vertex()->squareDistanceTo(*this->vertex()));
                dist_ctrl = (*it)->vertex()->squareDistanceTo(*this->vertex());
        }
#endif
        return find_nei;
}








