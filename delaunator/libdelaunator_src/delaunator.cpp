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
 * @param obj an object that is not already in Delaunator instance
 * @param coords place where object will be
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
TrianguledObject* Delaunator::addObject(TrianguledObject* obj, Coordinates coords) {
        return this->addObject(obj, coords.x(), coords.y());
}



/**
 * Place an object in the triangulation, at the given coords
 * @param obj an object that is not already in Delaunator instance
 * @param x coordinate in x-axis
 * @param y coordinate in y-axis
 * @return obj, just for facilitate functionnal approach
 * @note object is not, modified, free or moved in memory
 */
TrianguledObject* Delaunator::addObject(TrianguledObject* obj, float x, float y) {
        Vertex* new_vtx = this->triangulation->addVertexAt(x, y);
        obj->setVertex(new_vtx);
        this->object_count++;
        return obj;
}



/**
 * Remove given object of the triangulation.
 * @param obj a TrianguledObject that have been added before
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory, its just forgeted by Triangulation
 */
TrianguledObject* Delaunator::delObject(TrianguledObject* obj) {
        obj->setVertex(NULL);
        this->object_count--;
        return obj;
}



/**
 * Move given object by given values. (relative move)
 * @param obj a TrianguledObject that have been added before
 * @param relative_move used for movement
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
TrianguledObject* Delaunator::movObject(TrianguledObject* obj, Coordinates relative_move) {
#if DEBUG
        assert(obj->getVertex() != NULL);
#endif
        // if another object is referenced by obj Vertex, a new Vertex will be creat
        if(obj->confoundedWithAnotherObject()) {
                Vertex* new_vtx = this->triangulation->addVertexAt(
                                obj->getCoordinates() + relative_move, // vertex is creat at the right place
                                obj->getVertex()->getEdge() // this edge will be used by Finder
                );
                // obj will be add to new Vertex and forget by the ancient one
                obj->setVertex(new_vtx);
        // Else, we can just move Vertex at the new place
        } else {
                this->triangulation->moveVertex(obj->getVertex(), relative_move);
        }
        return obj;
}



/**
 * Move given object by given values. (relative move)
 * @param obj a TrianguledObject that have been added before
 * @param rel_x relative move in x-axis
 * @param rel_y relative move in y-axis
 * @return obj, just for facilitate functionnal approach
 * @note object is not free or moved in memory
 */
TrianguledObject* Delaunator::movObject(TrianguledObject* obj, float rel_x, float rel_y) {
        return this->movObject(obj, Coordinates(rel_x, rel_y));
}








/**
 * @param coords Coordinates where looking for
 * @return TrianguledObject found at given coords, or NULL if nothing found
 */
TrianguledObject* Delaunator::objectAt(Coordinates coords, float precision) const {
        return this->objectAt(coords.x(), coords.y(), precision);
}



/**
 * @param x coord in x-axis where looking for
 * @param y coord in y-axis where looking for
 * @return TrianguledObject found at given coords, or NULL if nothing found
 */
TrianguledObject* Delaunator::objectAt(float x, float y, float precision) const {
        Vertex* target = this->triangulation->vertexAt(x, y, precision);
        return target != NULL ? target->getFirstObject() : NULL;
}




/**
 * @param coords Coordinates where looking for
 * @return TrianguledObject list, found at given coords
 */
std::list<TrianguledObject*> Delaunator::objectsAt(Coordinates coords, float precision) const {
        return this->objectsAt(coords.x(), coords.y(), precision);
}



/**
 * @param x coord in x-axis where looking for
 * @param y coord in y-axis where looking for
 * @return TrianguledObject list, found at given coords
 */
std::list<TrianguledObject*> Delaunator::objectsAt(float x, float y, float precision) const {
        Vertex* target = this->triangulation->vertexAt(x, y, precision);
        return target != NULL ? target->getObjects() : std::list<TrianguledObject*>();
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





/***************************************************
 * PREDICATS
 ***************************************************/


/***************************************************
 * ITERATORS
 ***************************************************/
std::list<TrianguledObject*> Delaunator::objects() const {
        std::list<TrianguledObject*> lret;
        // for each Vertex of Triangulation
        IteratorOnVertices_read it = this->triangulation->iterVertices_read();
        while(it != it.end()) {
                // add TrianguledObject of Vertex to lret
                lret.splice(lret.end(), (*it)->getObjects());
                it++;
        }
        return lret;
        
}


/***************************************************
 * PRIVATE METHODS
 ***************************************************/




