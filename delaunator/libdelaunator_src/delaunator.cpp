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
#ifdef FOLLOW_SEARCH
        for(IteratorOnAllEdges it = this->allEdges(); it != it.end(); it++) {
                (*it)->passing = false;
        }
#endif
        container = this->findContainerOf(p);
        if(container != NULL) {
        
// divide in 3 triangles ( >container>, >p>, >new_vertex )
        // References edges of container
        Edge* p1_p2 = container->getEdge1();
        Edge* p2_p3 = container->getEdge2();
        Edge* p3_p1 = container->getEdge3();
#if DEBUG
        assert(p1_p2->leftFace() == container);
        assert(p2_p3->leftFace() == container);
        assert(p3_p1->leftFace() == container);
#endif
        // Creat the new vertex
        this->vertices.push_back(new Vertex(p));
        Vertex* v0 = new_vertex = this->vertices.back();
        Vertex* v1 = p1_p2->originVertex();
        Vertex* v2 = p2_p3->originVertex();
        Vertex* v3 = p3_p1->originVertex();
        // Creat new faces
        Face* f1 = container;
        Face* f2 = new Face(NULL);
        Face* f3 = new Face(NULL);
        this->faces.push_back(f2);
        this->faces.push_back(f3);
        // Creat new edges(originVertex, oppositeEdge, nextleftEdge)
        Edge* p0_p1 = new Edge(v0, NULL,  p1_p2);
        Edge* p0_p2 = new Edge(v0, NULL,  p2_p3); 
        Edge* p2_p0 = new Edge(v2, p0_p2, p0_p1);
        Edge* p0_p3 = new Edge(v0, NULL,  p3_p1);
        Edge* p3_p0 = new Edge(v3, p0_p3, p0_p2);
        Edge* p1_p0 = new Edge(v1, p0_p1, p0_p3); 
        this->edges.push_back(p0_p1);
        this->edges.push_back(p0_p2);
        this->edges.push_back(p2_p0);
        this->edges.push_back(p0_p3);
        this->edges.push_back(p3_p0);
        this->edges.push_back(p1_p0); 
        // Assign next left edges
        p1_p2->setNextLeftEdge(p2_p0);
        p2_p3->setNextLeftEdge(p3_p0);
        p3_p1->setNextLeftEdge(p1_p0);
        // Assign faces
        f1->setEdge(p2_p0); 
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
        //v->x() += vec_x;
        //v->y() += vec_y;
        v->setX(v->x() + vec_x);
        v->setY(v->y() + vec_y);
        if(v->x() < this->xmin)  v->setX(this->xmin);
        if(v->x() > this->xmax)  v->setX(this->xmax);
        if(v->y() < this->ymin)  v->setY(this->ymin);
        if(v->y() > this->ymax)  v->setY(this->ymax);

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
                if(fabs(x-current->x()) < (precision+EPSILON) && fabs(y-current->y()) < (precision+EPSILON))
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
        return !(p.x() < this->xmin || this->xmax < p.x() || p.y() < this->ymin || this->ymax < p.y());
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
                assert((*it)->getEdge()->leftFace()->collideAt(*(*it)));
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
IteratorVertexToNeighbourVertices Delaunator::getNeighbors(Vertex* v) {
        return IteratorVertexToNeighbourVertices(v);
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
Face* Delaunator::findContainerOf(Coordinates target) const {
// initialization
        Face* container = NULL;
        Edge *edge_cur = NULL;

#if DEBUG
        assert(this->collideAt(target));
#endif

// choose the initial Edge ( >edge> )
#ifdef DEULAUNAY_FINDER_INITIAL_RANDOM 
        edge_cur = this->edges[randN(this->edges.size())];
#else
#ifdef DEULAUNAY_FINDER_INITIAL_MIDDLE
        edge_cur = this->edges[this->edges.size() / 2];
#else
        edge_cur = this->edges[0];
#endif
#endif

// while face not found, search face ( >edge>, container> , >counter> )
#if DEBUG
        unsigned int iter_count = 0;
#endif
        short counter_left = 0;
        while(container == NULL) {
                Edge *edge_nxt = NULL;

                // if on the right of target
                if(edge_cur->coordOnTheLeft(target)) {
                        counter_left++; // the edge was the better way without rotation.
                        // churn until be on the left of target, 
                        do {
                                edge_cur = edge_cur->rotLeftEdge(); 
                        } while((not edge_cur->isVisible()) || edge_cur->coordOnTheStrictLeft(target));
                        if(edge_cur->coordOnTheStrictLeft(target)) {
                                do { 
                                        edge_cur = edge_cur->rotLeftEdge(); 
                                } while((not edge_cur->isVisible()) || edge_cur->coordOnTheStrictLeft(target));
                                counter_left = 0; // we are not currently running around the container.
                        }
                        // And then churn to right for get the better way.
                        edge_nxt = edge_cur->rotRightEdge();
                } else {
                        counter_left = 0; // we are not currently running around the container.
                        do { edge_cur = edge_cur->rotRightEdge(); 
                        } while((not edge_cur->isVisible()) || edge_cur->coordOnTheStrictRight(target));
                        // And then churn to left for get the better way.
                        edge_nxt = edge_cur;
                }
#if DEBUG
                assert(edge_nxt != NULL); // there is always an edge that go on the right of the target
                assert(edge_nxt->originVertex() == edge_cur->originVertex());
#endif

                // Jump to next edge
                edge_cur->passing = true;
                edge_cur = edge_nxt->nextLeftEdge();
                if(counter_left >= 3)   container = edge_cur->leftFace();


#if DEBUG
                assert((iter_count++) < (this->edges.size()*2+4));
#endif
        }
#if DEBUG
        assert(container->collideAt(target));
#endif

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
bool Delaunator::flipOn(Face* f_ref, unsigned int ttl) {
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
#ifdef FOLLOW_SEARCH
                illegal_edge1->passing= false;
                illegal_edge2->passing= false;
#endif
                // Recursiv call on the updated faces
                // protection against infinite recursive call.
                if(ttl < this->faces.size()/2) {
                        this->flipOn(f_ref, ttl+1);
                        this->flipOn(f_nei, ttl+1);
                } else {
                        logs("TTL %u reached !\n", ttl);
                }
        }


// ENDING
        return flip_done;
}








