#include "trianguledObject.h"



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
 * ACCESSORS
 ***************************************************/
/**
 * @return Vertex referenced by this
 */
Vertex* TrianguledObject::getVertex() const {
        return this->vertex;
}


/**
 * @param v the new Vertex that will be referenced by this.
 */
void TrianguledObject::setVertex(Vertex* v) {
        this->vertex = vertex;
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/







