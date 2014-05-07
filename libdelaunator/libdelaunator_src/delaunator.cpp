#include "delaunator.h"



/***************************************************
 * CONSTRUCTOR
 ***************************************************/
Delaunator::Delaunator(const float xmin, const float xmax, 
                const float ymin, const float ymax) : xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax) {
// Creation of primitive mesh, with four points.
        this->vertices.push_back(new Vertex(xmin, ymin)); // NORTH-WEST
        this->vertices.push_back(new Vertex(xmax, ymin)); // NORTH-EAST
        this->vertices.push_back(new Vertex(xmax, ymax)); // SOUTH-EAST
        this->vertices.push_back(new Vertex(xmin, ymax)); // SOUTH-WEST

// Creation of Edges that rely vertices.
        this->edges.push_back(new Edge(this->vertices[0]));
        this->edges.push_back(new Edge(this->vertices[0]));
        this->edges.push_back(new Edge(this->vertices[0]));
        Edge* eNW2NE = this->edges[0];
        Edge* eNW2SE = this->edges[1];
        Edge* eNW2SW = this->edges[2];

        this->edges.push_back(new Edge(this->vertices[1]));
        this->edges.push_back(new Edge(this->vertices[1]));
        this->edges.push_back(new Edge(this->vertices[1]));
        Edge* eNE2NW = this->edges[3];
        Edge* eNE2SE = this->edges[4];
        Edge* eNE2SW = this->edges[5];

        this->edges.push_back(new Edge(this->vertices[2]));
        this->edges.push_back(new Edge(this->vertices[2]));
        this->edges.push_back(new Edge(this->vertices[2]));
        Edge* eSE2NW = this->edges[6];
        Edge* eSE2NE = this->edges[7];
        Edge* eSE2SW = this->edges[8];

        this->edges.push_back(new Edge(this->vertices[3]));
        this->edges.push_back(new Edge(this->vertices[3]));
        this->edges.push_back(new Edge(this->vertices[3]));
        Edge* eSW2NW = this->edges[9];
        Edge* eSW2NE = this->edges[10];
        Edge* eSW2SE = this->edges[11];

        //non-visible edges: they passed "behind" the plan.
        eNW2SE->setVisibility(false);
        eSE2NW->setVisibility(false);

// Set attribute member of edges
        eNW2NE->setNextLeftEdge(eNE2SE);
        eNW2NE->setOppositeEdge(eNE2NW);

        eNW2SE->setNextLeftEdge(eSE2SW);
        eNW2SE->setOppositeEdge(eSE2NW);

        eNW2SW->setNextLeftEdge(eSW2NE);
        eNW2SW->setOppositeEdge(eSW2NW);


        eNE2NW->setNextLeftEdge(eNW2SW);
        eNE2NW->setOppositeEdge(eNW2NE);

        eNE2SE->setNextLeftEdge(eSE2NW);
        eNE2SE->setOppositeEdge(eSE2NE);

        eNE2SW->setNextLeftEdge(eSW2SE);
        eNE2SW->setOppositeEdge(eSW2NE);


        eSE2NW->setNextLeftEdge(eNW2NE);
        eSE2NW->setOppositeEdge(eNW2SE);

        eSE2NE->setNextLeftEdge(eNE2SW);
        eSE2NE->setOppositeEdge(eNE2SE);

        eSE2SW->setNextLeftEdge(eSW2NW);
        eSE2SW->setOppositeEdge(eSW2SE);


        eSW2NW->setNextLeftEdge(eNW2SE);
        eSW2NW->setOppositeEdge(eNW2SW);

        eSW2NE->setNextLeftEdge(eNE2NW);
        eSW2NE->setOppositeEdge(eNE2SW);

        eSW2SE->setNextLeftEdge(eSE2NE);
        eSW2SE->setOppositeEdge(eSE2SW);
#if DEBUG
        assert(eSW2SE == this->edges[11]);
        assert(eNW2NE == this->edges[0]);
        assert(eNW2NE->oppositeEdge() == eNE2NW);
#endif

// Creation of the four faces. (two external faces exist, but are not used, for simplify treatments)
        this->faces.push_back(new Face(eSW2NE));
        this->faces.push_back(new Face(eNE2SW));
        this->faces.push_back(new Face(eSW2NW, false)); // extern face, describe outer world
        this->faces.push_back(new Face(eNE2SE, false)); // extern face, describe outer world

#if DEBUG
        assert(eSW2NE->leftFace() == this->faces[0]);
        assert(eNE2SW->leftFace() == this->faces[1]);
        assert(eSE2SW->leftFace() == this->faces[2]);
        assert(eNE2SE->leftFace() == this->faces[3]);
#endif
}



Delaunator::~Delaunator() {
        // free's !
        for(Face* it : this->faces) {
                delete it;
        }
        for(Vertex* it : this->vertices) {
                delete it;
        }
        for(Edge* it : this->edges) {
                delete it;
        }
}






/***************************************************
 * PUBLIC METHODS
 ***************************************************/

/*
 * ADD VERTEX
 */
// Add a new vertex to Mesh at given coordinates.
// Return address of that point in Mesh, or NULL if not found or out of bounds.
Vertex* Delaunator::addVertexAt(Coordinates p) {
// initialization
        Face* container = NULL; // container of p
        Vertex* new_vertex = NULL; // returned vertex
        if(this->collideAt(p)) {

// find container of vertex ( container>, >p )
        container = this->findContainerOf(p);
        if(container != NULL) {
        
// divide in 3 triangles ( >container>, >p>, >new_vertex )
        // References edges of container
        Edge* p1_p2 = container->getEdge1();
        Edge* p2_p3 = container->getEdge2();
        Edge* p3_p1 = container->getEdge3();
        // Creat the new vertex
        this->vertices.push_back(new Vertex(p));
        Vertex* v0 = new_vertex = this->vertices.back();
        Vertex* v1 = p1_p2->originVertex();
        Vertex* v2 = p2_p3->originVertex();
        Vertex* v3 = p3_p1->originVertex();
        // Creat new faces
        Face* f1 = container;
        this->faces.push_back(new Face(NULL));
        Face* f2 = this->faces.back();
        this->faces.push_back(new Face(NULL));
        Face* f3 = this->faces.back();
        // Creat new edges
        this->edges.push_back(new Edge(v0, NULL, p1_p2));
        Edge* p0_p1 = this->edges.back();
        this->edges.push_back(new Edge(v1, p0_p1, NULL)); // next left edge assigned few lines after
        Edge* p1_p0 = this->edges.back();
        this->edges.push_back(new Edge(v0, NULL, p2_p3));
        Edge* p0_p2 = this->edges.back();
        this->edges.push_back(new Edge(v2, p0_p2, p0_p1));
        Edge* p2_p0 = this->edges.back();
        this->edges.push_back(new Edge(v0, NULL, p3_p1));
        Edge* p0_p3 = this->edges.back();
        this->edges.push_back(new Edge(v3, p0_p3, p0_p2));
        Edge* p3_p0 = this->edges.back();
        // Assign next left edges
        p1_p2->setNextLeftEdge(p2_p0);
        p2_p3->setNextLeftEdge(p3_p0);
        p3_p1->setNextLeftEdge(p1_p0);
        p1_p0->setNextLeftEdge(p0_p3);
        // Assign faces
        f1->setEdge(p2_p0); // p1_p2 is already set to f1. If we use setEdge(p0_p1), p2_p0 will never be updated.
        f2->setEdge(p3_p0);
        f3->setEdge(p1_p0);
#if DEBUG
        this->DEBUG_tests();
#endif

#if DELAUNAY_CONDITION
// apply delaunay restrictions
        this->flipOn(f1);
        this->flipOn(f2);
        this->flipOn(f3);
#endif
// ending
        }}
        return new_vertex;
}




/*
 * MOVE VERTEX
 */
// Move given vertex by given vector values.
// Modify the mesh in consequence.
// Vertex can't pass over the mesh bounds.
void Delaunator::moveVertex(Vertex* v, float vec_x, float vec_y) {
// Move the vertex
        v->x += vec_x;
        v->y += vec_y;
        if(v->x < this->xmin)  v->x = this->xmin;
        if(v->x > this->xmax)  v->x = this->xmax;
        if(v->y < this->ymin)  v->y = this->ymin;
        if(v->y > this->ymax)  v->y = this->ymax;

// Detect if outside the face
        

// Modify Face 
#if DELAUNAY_CONDITION
// Apply Delaunay Condition
        std::vector<Face*> nei_faces;
        Edge* edge = v->getEdge();
        do {
                nei_faces.push_back(edge->leftFace());
                edge = edge->rotLeftEdge();
        } while(edge != v->getEdge());

        for(Face* f : nei_faces) {
#if DEBUG
                assert(f != NULL);
#endif
                this->flipOn(f);
        }
#endif 
}





/*
 * VERTEX AT
 */
// Return vertex found at given coords, around precision.
// Return NULL if no vertex found.
Vertex* Delaunator::vertexAt(float x, float y, float precision) const {
        Vertex *target = NULL, *current = NULL;
        for(IteratorOnAllVertices_read it = this->allVertices_read(); target == NULL && it != it.end(); it++) {
                current = *it;
                if(fabs(x-current->x) < (precision+EPSILON) && fabs(y-current->y) < (precision+EPSILON))
                        target = current;
        }
        //if(target != NULL) {
                //logs("TARGET: %u, have %u neighbors.\n", target->getID(), target->neighbourCount());
        //}
        return target;
}





/*
 * COLLIDE AT
 */
// Return true if given coordinates are in-limit of this.
bool Delaunator::collideAt(Coordinates p) const {
        return !(p.x < this->xmin || this->xmax < p.x || p.y < this->ymin || this->ymax < p.y);
}




#if DEBUG // some tests with assertions
/*
 * DEBUG TESTS
 */
void Delaunator::DEBUG_tests() const {
        for(IteratorOnAllEdges_read it = this->allEdges_read(); it != it.end(); it++) {
                assert((*it)->originVertex() != NULL);
                assert((*it)->oppositeEdge() != NULL);
                assert((*it)->leftFace() != NULL);
                assert((*it)->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == (*it));
        }
        for(IteratorOnAllFaces_read it = this->allFaces_read(); it != it.end(); it++) {
                assert((*it)->getEdge() != NULL);
        }
        for(IteratorOnAllVertices_read it = this->allVertices_read(); it != it.end(); it++) {
                assert((*it)->getEdge() != NULL);
                assert((*it)->getEdge()->originVertex() == (*it));
        }
}
#endif





/***************************************************
 * ITERATORS
 ***************************************************/
/*
 * VERTEX TO NEIGHBOUR VERTICES
 */
// Infinite iteration on all neighbour vertices of a given Vertex.
IteratorVertexToNeighbourVertices* Delaunator::getIteratorVertexToNeighbourVertices(Vertex* v) {
        return new IteratorVertexToNeighbourVertices(v);
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/
/*
 * FIND CONTAINER OF COORDINATES
 */
// Return address of Face of Delaunator that contains given coordinates.
// Coordinates must be != NULL and contained in bounds of Delaunator.
// Return NULL if error. If out of bounds, return the unvisible face that contain p.
Face* Delaunator::findContainerOf(Coordinates p) const {
// initialization
        Face* container = NULL;
        short counter = 0; // turn around a face == 3 times the good way is the nextLeftEdge().
        Edge* edge = NULL;

#if DEBUG
        assert(this->collideAt(p));
#endif

// choose the initial Edge ( >edge> )
#ifdef DEULAUNAY_FINDER_INITIAL_RANDOM 
        edge = this->edges[randN(this->edges.size())];
#else
#ifdef DEULAUNAY_FINDER_INITIAL_MIDDLE
        edge = this->edges[this->edges.size() / 2];
#else
        edge = this->edges[0];
#endif
#endif

// while face not found, search face ( >edge>, container> , >counter> )
#if DEBUG
        unsigned int iter_count = 1;
#endif
        while(container == NULL) {
                //fprintf(stderr,"Move %i on edge %i for coords (%f;%f):\n", iter_count, edge->getID(), p.x, p.y);
                if(edge->coordOnTheStrictRight(p)) {
                        //fprintf(stderr,"\t-on the right of edge %i, ", edge->getID());
                        // there is probably a better way
                        edge = edge->rotRightEdge();
                        //while(!edge->leftFace()->isVisible()) edge = edge->rotRightEdge();
                        //fprintf(stderr,"change to edge %i, set counter to 0;\n", edge->getID());
                        counter = 0; // we don't turn around a face yet.
                } else {
#if DEBUG
                        iter_count++;
#endif
                        // we found the way that drive on the left of the coordinates,
                        // so better way is the previous one, lst_way.
                        // if 
                        // We jump to this way.
                        edge = edge->nextLeftEdge();
                        //while(!edge->leftFace()->isVisible()) edge = edge->rotRightEdge();
                        if(++counter == 3) {
                                // Face found !
                                container = edge->leftFace();
                        }
                        //fprintf(stderr,"\t-on the left, change to edge %u", edge->getID());
                        //fprintf(stderr,", with counter at %i;\n", counter);
                }
#if DEBUG
                assert(iter_count < this->edges.size()*1 + 1); 
#endif
        }
        //logs("FOUND IN %u MOVES.\n", iter_count);
        //logs("FOUND IN %u MOVES; FACE FOUND: %u\n", iter_count, container-getID());

// ending
        return container;
}






/*
 * FLIP ON
 */
// Operate Delaunay Flip Algorithm on Face if necessary.
// Face can't be NULL and must be integrate in triangulation. (== have a valid edge field)
// Return true if modifications.
// Recursiv call on new triangles created by flip.
bool Delaunator::flipOn(Face* f_ref) {
#if DEBUG
        assert(f_ref != NULL && f_ref->getEdge() != NULL);
#endif
// INITIALIZATION
        bool flip_done = true; // we are optimist.
        Face* f_nei = NULL; // neighbour of f_ref that have an illegal side with it.
        Edge* illegal_edge1 = NULL; // illegal edge that relie the illegal vertex and the f_ref's vertex.
        Edge* illegal_edge2 = NULL; // and it's opposite.
        Vertex* illegal_vertex1 = NULL; // vertex that is illegal because of illegal edge.
        Vertex* illegal_vertex2 = NULL; // idem. these two vertices will be linked by edge after flip.


// CONDITION: in circumcircle
        // FIRST NEIGHBOUR
        if(f_ref->circumcircleContainCoords(*f_ref->getEdge1()->nextRightEdge()->destinVertex())) {
                illegal_edge1 = f_ref->getEdge1();
                illegal_vertex1 = f_ref->getP3();

        // SECOND NEIGHBOUR
        } else if(f_ref->circumcircleContainCoords(*f_ref->getEdge2()->nextRightEdge()->destinVertex())) {
                illegal_edge1 = f_ref->getEdge2();
                illegal_vertex1 = f_ref->getP1();

        // THIRD NEIGHBOUR
        } else if(f_ref->circumcircleContainCoords(*f_ref->getEdge3()->nextRightEdge()->destinVertex())) {
                illegal_edge1 = f_ref->getEdge3();
                illegal_vertex1 = f_ref->getP2();

        // NO ILLEGAL SIDE
        } else  flip_done = false;


// MODIFICATION OF FACES
        if(flip_done) {

                // Deduce some shortcuts
                illegal_edge2 = illegal_edge1->oppositeEdge();
                illegal_vertex2 = illegal_edge1->nextRightEdge()->destinVertex();
                f_nei = illegal_edge2->leftFace();
                // previous and next edge in the cycle that form the Face.
                Edge* edge1_prev = illegal_edge1->nextLeftEdge();
                Edge* edge1_next = illegal_edge1->nextRightEdge()->oppositeEdge();
                Edge* edge2_prev = illegal_edge2->nextLeftEdge();
                Edge* edge2_next = illegal_edge2->nextRightEdge()->oppositeEdge();

#if DEBUG
                assert(illegal_edge1->originVertex() != illegal_vertex1);
                assert(illegal_edge2->originVertex() != illegal_vertex1);
                assert(illegal_edge1->originVertex() != illegal_vertex2);
                assert(illegal_edge2->originVertex() != illegal_vertex2);
#endif

                // Origin vertex of illegal edges can't reference illegal edges anymore
                illegal_edge1->originVertex()->setEdge(edge2_prev);
                illegal_edge2->originVertex()->setEdge(edge1_prev);

                // Modify next left and right edges of non-illegal edges
                // NB: use of inter vars because change the next left edge will change the result of nextRightEdge.
                edge1_prev->setNextLeftEdge(illegal_edge1);
                edge2_prev->setNextLeftEdge(illegal_edge2);

                edge1_next->setNextLeftEdge(edge1_prev);
                edge2_next->setNextLeftEdge(edge2_prev);

                illegal_edge1->setNextLeftEdge(edge1_next); // Close the cycle
                illegal_edge2->setNextLeftEdge(edge2_next); 


                // assign origin vertex
                illegal_edge1->setOriginVertex(illegal_vertex1);
                illegal_edge2->setOriginVertex(illegal_vertex2);

                // update faces
                f_ref->setEdge(illegal_edge1);
                f_nei->setEdge(illegal_edge2);

#if DEBUG
                assert(illegal_edge1->originVertex() == illegal_vertex1);
                assert(illegal_edge1->nextLeftEdge() == illegal_edge2->nextRightEdge()->nextRightEdge()->oppositeEdge());
                assert(illegal_edge1->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == illegal_edge1);
                assert(illegal_edge1->leftFace() == f_ref);
                assert(illegal_edge1->nextLeftEdge()->leftFace() == f_ref);
                assert(illegal_edge1->nextLeftEdge()->nextLeftEdge()->leftFace() == f_ref);

                assert(illegal_edge2->destinVertex() == illegal_vertex1);
                assert(illegal_edge2->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == illegal_edge2);
                assert(illegal_edge2->leftFace() == f_nei);
                assert(illegal_edge2->nextLeftEdge()->leftFace() == f_nei);
                assert(illegal_edge2->nextLeftEdge()->nextLeftEdge()->leftFace() == f_nei);

                this->DEBUG_tests();
#endif
                // Recursiv call on the updated faces
                this->flipOn(f_ref);
                this->flipOn(f_nei);
        }


// ENDING
        return flip_done;
}








