#include "delaunator.h"



/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Wait for limits of triangulation in axis x and y.
 * @param xmin minimal value in x axis allowed
 * @param xmax maximal value in x axis allowed
 * @param ymin minimal value in y axis allowed
 * @param ymax maximal value in y axis allowed
 * @param finder_mode for choose which mode of finder will be used for search Coordinates, LAST by default
 * @see VertexFinderMode
 */
Delaunator::Delaunator(const float xmin, const float xmax, 
                       const float ymin, const float ymax, 
                       const VertexFinderMode finder_mode) {
        this->triangulation = new Triangulation(xmin, xmax, ymin, ymax, finder_mode);
        object_count = 0;
}



/**
 * Frees
 */
Delaunator::~Delaunator() {
        delete this->triangulation;
}






/***************************************************
 * PUBLIC METHODS
 ***************************************************/
/**
 * Place an object in the triangulation, at the given coords
 * @param coords place where object will be
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
VirtualVertex* Delaunator::addVirtualVertex(Coordinates coords) {
        return this->addVirtualVertex(coords.x(), coords.y());
}



/**
 * Place an object in the triangulation, at the given coords
 * @param x coordinate in x-axis
 * @param y coordinate in y-axis
 * @return new VirtualVertex object address, or NULL if invalid coords
 * @note no modifications if given coords are invalids (out of bounds)
 */
VirtualVertex* Delaunator::addVirtualVertex(float x, float y) {
        Vertex* new_vtx = this->triangulation->addVertexAt(x, y);       // create or get a vertex
        VirtualVertex* obj = NULL;
        if(new_vtx != NULL) {
                obj = new VirtualVertex(new_vtx);                       // create a virtual vertex
                new_vtx->take(obj);                                     // link it to real vertex
                this->object_count++;                                   // one object more !
        }
        return obj;                                                     // give virtual vertex ref to user
}



/**
 * Remove given object of the triangulation.
 * @param obj a VirtualVertex that have been added before
 * @return obj, just for facilitate functionnal approach
 * @note given VirtualVertex is deleted
 */
void Delaunator::delVirtualVertex(VirtualVertex* obj) {
        Vertex* vtx = obj->vertex();
        if(vtx != NULL) {
                vtx->forget(obj);
                if(vtx->getObjectCount() == 0) 
                        this->triangulation->delVertex(vtx);
        }
        delete obj;
        this->object_count--;
}



/**
 * Move given object by given values. (relative move)
 * @param obj a VirtualVertex that have been added before
 * @param relative_move used for movement
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
VirtualVertex* Delaunator::movVirtualVertex(VirtualVertex* obj, Coordinates relative_move) {
#ifdef DEBUG
        assert(obj->vertex() != NULL);
#endif
        // if another object is referenced by obj Vertex, a new Vertex will be creat
        if(obj->confoundedWithAnotherObject()) {
                Vertex* new_vtx = this->triangulation->addVertexAt(
                                obj->coordinates() + relative_move, // vertex is creat at the right place
                                obj->vertex()->getEdge() // this edge will be used by Finder
                );
                // obj will be add to new Vertex and forget by the ancient one
                new_vtx->take(obj, obj->vertex());
#ifdef DEBUG
                assert(new_vtx->getObjectCount() > 0); // not == 1, because new_vtx can be an pre-existing vertex
                assert(obj->vertex()->getObjectCount() > 0);
#endif
        // Else, we can just move Vertex at the new place
        } else {
                this->triangulation->moveVertex(obj->vertex(), relative_move);
        }
        return obj;
}



/**
 * Move given object by given values. (relative move)
 * @param obj a VirtualVertex that have been added before
 * @param rel_x relative move in x-axis
 * @param rel_y relative move in y-axis
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
VirtualVertex* Delaunator::movVirtualVertex(VirtualVertex* obj, float rel_x, float rel_y) {
        return this->movVirtualVertex(obj, Coordinates(rel_x, rel_y));
}








/**
 * @param coords Coordinates where looking for
 * @return VirtualVertex found at given coords, or NULL if nothing found
 */
VirtualVertex* Delaunator::virtualVertexAt(Coordinates coords, float precision) const {
        return this->virtualVertexAt(coords.x(), coords.y(), precision);
}



/**
 * @param x coord in x-axis where looking for
 * @param y coord in y-axis where looking for
 * @return VirtualVertex found at given coords, or NULL if nothing found
 */
VirtualVertex* Delaunator::virtualVertexAt(float x, float y, float precision) const {
        Vertex* target = this->triangulation->vertexAt(x, y, precision);
        return target != NULL ? target->getFirstObject() : NULL;
}




/**
 * @param coords Coordinates where looking for
 * @return VirtualVertex list, found at given coords
 */
std::list<VirtualVertex*> Delaunator::virtualVerticesAt(Coordinates coords, float precision) const {
        return this->virtualVerticesAt(coords.x(), coords.y(), precision);
}



/**
 * @param x coord in x-axis where looking for
 * @param y coord in y-axis where looking for
 * @return VirtualVertex list, found at given coords
 */
std::list<VirtualVertex*> Delaunator::virtualVerticesAt(float x, float y, float precision) const {
        Vertex* target = this->triangulation->vertexAt(x, y, precision);
        return target != NULL ? target->getObjects() : std::list<VirtualVertex*>();
}









/***************************************************
 * ACCESSORS
 ***************************************************/
/**
 * @param v tested Vertex 
 * @return true iff tested Vertex is referenced by triangulation
 */
bool Delaunator::haveVertex(Vertex* v) const {
        return this->triangulation->haveVertex(v);
}




/**
 * @param c Coordinates of tested point
 * @return true if point is in-limit of this
 */
bool Delaunator::collideAt(Coordinates c) const {
        return this->triangulation->collideAt(c);
}


/**
 * @return the maximum distance reachable in triangulation
 * @note that distance is equal to distance between (xmin;ymin) and (xmax;ymax)
 */
float Delaunator::distanceMax() const {
        return Coordinates(this->getXmin(), this->getYmin()).distanceTo(
                        Coordinates(this->getXmax(), this->getYmax())
                );
} 



/***************************************************
 * PREDICATS
 ***************************************************/


/***************************************************
 * ITERATORS
 ***************************************************/
std::list<VirtualVertex*> Delaunator::virtualVertices() const {
        std::list<VirtualVertex*> lret;
        // for each Vertex of Triangulation
        IteratorOnVertices_read it = this->triangulation->iterVertices_read();
        while(it != it.end()) {
                // add VirtualVertex of Vertex to lret
                lret.splice(lret.end(), (*it)->getObjects());
                it++;
        }
        return lret;
        
}


/***************************************************
 * PRIVATE METHODS
 ***************************************************/




