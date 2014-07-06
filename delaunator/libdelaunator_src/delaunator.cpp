#include "delaunator.h"



/***************************************************
 * CONSTRUCTOR
 ***************************************************/
/**
 * Wait for limits of triangulation in axis x and y.
 */
Delaunator::Delaunator(const float xmin, const float xmax, 
                       const float ymin, const float ymax, Delaunator::FinderInitial finder_mode) : 
                                xmin(xmin), xmax(xmax), ymin(ymin), ymax(ymax), finder_mode(finder_mode) {
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
#ifdef DEBUG
        assert(eSW2SE == this->edges[11]);
        assert(eNW2NE == this->edges[0]);
        assert(eNW2NE->oppositeEdge() == eNE2NW);
#endif

// Creation of the four faces. (two external faces exist, but are not used, for simplify treatments)
        this->faces.push_back(new Face(eSW2NE));
        this->faces.push_back(new Face(eNE2SW));
        this->faces.push_back(new Face(eSW2NW, false)); // extern face, describe outer world
        this->faces.push_back(new Face(eNE2SE, false)); // extern face, describe outer world

#ifdef DEBUG
        assert(eSW2NE->leftFace() == this->faces[0]);
        assert(eNE2SW->leftFace() == this->faces[1]);
        assert(eSE2SW->leftFace() == this->faces[2]);
        assert(eNE2SE->leftFace() == this->faces[3]);
        this->DEBUG_tests();
#endif
}



/**
 * Frees all Faces, Vertices and Edges.
 */
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
/**
 * Add a new vertex to Mesh at given coordinates.
 * Return address of that point in Mesh, or NULL if not found or out of bounds.
 */
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
#ifdef DEBUG
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
#ifdef DEBUG
        this->DEBUG_tests();
#endif

// apply delaunay restrictions
        this->applyDelaunayCondition(f1);
        this->applyDelaunayCondition(f2);
        this->applyDelaunayCondition(f3);
// ending
        }}
        return new_vertex;
}







/**
 * Move given vertex by given vector values.
 * Modify the mesh in consequence.
 * Vertex can't pass over the mesh bounds.
 * @param mv_vrtx the Vertex to move
 * @param vec_x move in x axis
 * @param vec_y move in y axis
 */
void Delaunator::moveVertex(Vertex* v, float x, float y) { 
        this->moveVertexTo(v, Coordinates(v->x()+x, v->y()+y)); 
}



/**
 * Move given vertex at given Coordinates.
 * Modify the mesh in consequence.
 * Vertex can't pass over the mesh bounds.
 * @param mv_vrtx the Vertex to move
 * @param new_position of the Vertex
 */
void Delaunator::moveVertexTo(Vertex* mv_vrtx, Coordinates new_position) {
// INIT
        Edge* col_edge = NULL;
        Coordinates circumcenter;
        // some containers
        std::vector<Edge*> limiter_edges;

        if((*mv_vrtx) != new_position) {
// LIMIT MOVE
                logs("LIMIT MOVE\n");
                if(new_position.x() < this->xmin)  new_position.setX(this->xmin);
                if(new_position.x() > this->xmax)  new_position.setX(this->xmax);
                if(new_position.y() < this->ymin)  new_position.setY(this->ymin);
                if(new_position.y() > this->ymax)  new_position.setY(this->ymax);

// FIND LIMITER EDGES 
                logs("FIND LIMITER EDGES\n");
                // limiter edges are the next left edges of all edge that have mv_vrtx as origin.
                col_edge = mv_vrtx->getEdge();
                do {
                        limiter_edges.push_back(col_edge->nextLeftEdge());
                        col_edge = col_edge->rotLeftEdge();
                } while(col_edge != mv_vrtx->getEdge());

// DETECT COLLISION WITH LIMITER EDGES
                logs("DETECT COLLISION WITH %i LIMITER EDGES\n", limiter_edges.size());
                // if no collision, just run the move pure
                col_edge = NULL;
                for(std::vector<Edge*>::iterator it = limiter_edges.begin(); it != limiter_edges.end(); it++) {
                        if((*(*it)->originVertex()) == new_position) {
                                it = limiter_edges.end();
                        } else if(geometry::collisionBetweenSegmentAndSegment(
                                        *(*it)->destinVertex(), *(*it)->originVertex(),
                                        *mv_vrtx, new_position)
                                && !geometry::parallelsLines(
                                        *(*it)->destinVertex(), *(*it)->originVertex(),
                                        *mv_vrtx, new_position)
                                && !isExternalEdge(*it)
                                        ) {
                                                col_edge = (*it); 
                                                it = limiter_edges.end()-1;
                        }
                }

// OPERATE MOVING
                logs("OPERATE MOVING BY %p\n", col_edge);
                // Verify collision with neighbour edge
                if(col_edge == NULL) {
                        // No collision found: we just move vertex and verify Delaunay condition
                        this->moveVertex_pure(mv_vrtx, new_position);
                } else {
                        // A collision is found !
                        Coordinates intersec = geometry::intersectionOfLines(
                                        *col_edge->destinVertex(), *col_edge->originVertex(),
                                        *mv_vrtx, new_position
                                        );
                        if((*mv_vrtx) != new_position && intersec != new_position) {
                                this->moveVertex_pure(mv_vrtx, intersec);
                                this->moveVertexTo(mv_vrtx, new_position);
                        }
                }
// ENDING
        }
}







/**
 * Return vertex found at given coords, around precision.
 * Return NULL if no vertex found.
 */
Vertex* Delaunator::vertexAt(float x, float y, float precision) const {
        Vertex *target = NULL, *current = NULL;
        for(IteratorOnAllVertices_read it = this->allVertices_read(); target == NULL && it != it.end(); it++) {
                current = *it;
                if(fabs(x-current->x()) < (precision+EPSILON) && fabs(y-current->y()) < (precision+EPSILON))
                        target = current;
        }
        return target;
}




/**
 * Remove a Vertex from the triangulation.
 * Iterators will be invalidated, and vertex will be free.
 * @param del_vrtx targeted Vertex
 */
void Delaunator::delVertex(Vertex* del_vrtx) {
// INIT
#ifdef DEBUG
        assert(del_vrtx != NULL);
        assert(this->haveVertex(del_vrtx));
        assert(!this->isCornerVertex(del_vrtx));
#endif
        bool modification = true; // false when no modification operate on triangulation
        // Creat some container
        std::vector<Face*> modified_faces(0); // modified faces that can break Delaunay condition
        std::vector<Edge*> nei_edge(0);
        std::vector<float> nei_dist(0);
        std::vector<Vertex*> nei_vrtx(0);
        // Fill those that refer neighbors, edge to neighbors or distance to neighbors
        Edge* edge = del_vrtx->getEdge();
        do {    nei_edge.push_back(edge);
                nei_vrtx.push_back(edge->destinVertex());
                nei_dist.push_back(edge->destinVertex()->distanceTo(*del_vrtx));
                edge = edge->rotLeftEdge();
        } while(edge != del_vrtx->getEdge());

// SIMPLIFY LOCAL TRIANGULATION
        // When we can't do something, it's when the del_vrtx is linked to tree points exactly.
        // At this time of the algo, Delaunay condition is breaked, and suppression of the point
        // is easy. 
        while(modification) {
                modification = false; 
                // For each neighbour
                for(unsigned int target = 0; target < nei_edge.size(); target++) {
                        // take tree consecutivs neighbors indexes
                        unsigned int id1 = target;
                        unsigned int id2 = (target+1) % nei_dist.size();
                        unsigned int id3 = (target+2) % nei_dist.size();
                        // collision between del_vrtx and triangle formed by neighbors
                        bool del_vrtx_in_triangle = geometry::pointInTriangle(
                                        *nei_vrtx[id1], *nei_vrtx[id2], *nei_vrtx[id3], *del_vrtx
                        );
                        // if middle point is farthest than at least one of the two others from del_vrtx
                        //      AND del_vrtx is not in triangle formed by the tree neighbors
                        if((nei_dist[id2] >= nei_dist[id1] || nei_dist[id2] >= nei_dist[id3]) 
                                        && !del_vrtx_in_triangle) {
                                // these tree points will be a triangle !
                                // so, operate flip on the middle edge
                                this->operateFlip(nei_edge[id2]);
                                modification  = true;
                                // remove middle vertex (id2) from neighbors lists
                                nei_edge.erase(nei_edge.begin() + id2);
                                nei_dist.erase(nei_dist.begin() + id2);
                                nei_vrtx.erase(nei_vrtx.begin() + id2);
                                // The modified faces must be referenced : maybe it break the Delaunay condition
                                modified_faces.push_back(nei_edge[id1]->nextLeftEdge()->rightFace());
                        }
                }
        }
        
// DELETE POINT FROM TRIANGLE CONTAINER
        // Container of del_vrtx is composed by the tree neighbors contains in nei_vrtx
        // Delete del_vrtx is exactly the reverse of adding, after find the container.
        Edge *edge1 = nei_edge.front();
        Edge *edge2 = edge1->rotLeftEdge();
        Edge *edge3 = edge2->rotLeftEdge();
        // f2 and f3 will be deleted. f1 is the future Face of triangle
        Face *f1 = edge1->leftFace(), *f2 = edge2->leftFace(), *f3 = edge3->leftFace();
        // sides are the edge that are the sides of the final triangle of face f1
        Edge *side1l = edge1->nextLeftEdge();
        Edge *side2l = edge2->nextLeftEdge();
        Edge *side3l = edge3->nextLeftEdge();
#ifdef DEBUG
        // Some tests
        assert(nei_vrtx.size() == 3);
        assert(edge3  != edge1  && edge1  != edge2  && edge2  != edge3 );
        assert(side3l != side1l && side1l != side2l && side2l != side3l);
        assert(f1 != f3 && f3 != f2 && f2 != f1);
        this->DEBUG_tests();
#endif
        // Origin Vertices must refers sides, no edge1, edge2 or edge3
        side1l->originVertex()->setEdge(side1l);
        side2l->originVertex()->setEdge(side2l);
        side3l->originVertex()->setEdge(side3l);
        // Set next left edge
        side1l->setNextLeftEdge(side2l);
        side2l->setNextLeftEdge(side3l);
        side3l->setNextLeftEdge(side1l);
        // Set face's Edge reference to a valide side, and reference it as a modified face
        f1->setEdge(side1l);
        modified_faces.push_back(f1);
        // Delete unwanted faces, edges, and finally del_vrtx
        this->removeEdgeFromEdges(edge1->oppositeEdge());
        this->removeEdgeFromEdges(edge2->oppositeEdge());
        this->removeEdgeFromEdges(edge3->oppositeEdge());
        this->removeEdgeFromEdges(edge1);
        this->removeEdgeFromEdges(edge2);
        this->removeEdgeFromEdges(edge3);
        this->removeFaceFromFaces(f2);
        this->removeFaceFromFaces(f3);
        this->removeVertexFromVertices(del_vrtx);

// RESTORE DELAUNAY CONDITION
        // Delaunay condition was break. It's time to restore it.
        for(Face* face : modified_faces) 
                this->applyDelaunayCondition(face);

// END
#ifdef DEBUG
        this->DEBUG_tests();
#endif
}




#ifdef DEBUG // some tests with assertions
/*
 * DEBUG TESTS.
 */
void Delaunator::DEBUG_tests() const {
        for(IteratorOnAllEdges_read it = this->allEdges_read(); it != it.end(); it++) {
                assert((*it)->originVertex() != NULL);
                assert((*it)->oppositeEdge() != NULL);
                assert((*it)->leftFace() != NULL);
                if((*it)->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() != (*it)) {
                        logs("%u, ", (*it)->getID());
                        logs("%u, ", (*it)->nextLeftEdge()->getID());
                        logs("%u, ", (*it)->nextLeftEdge()->nextLeftEdge()->getID());
                        logs("%u, ", (*it)->nextLeftEdge()->nextLeftEdge()->nextLeftEdge()->getID());
                        assert((*it)->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == (*it));
                }
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
 * ACCESSORS
 ***************************************************/
/**
 * Return index of a Vertex in Delaunator container.
 * @param v Vertex that will be found
 * @return an unsigned int that is the index of the Vertex for this Deulaunator, or equal to number of Vertice if not found
 */
unsigned int Delaunator::getIndexOf(Vertex* v) const {
        unsigned int index = this->vertices.size();
        for(unsigned int i = this->vertices.size() - 1; i >= 0 
                        && index == this->vertices.size(); i--) 
                if(this->vertices[i] == v)
                        index = i;
        return index;
}










/***************************************************
 * PREDICATS
 ***************************************************/
/**
 * @param v tested Vertex 
 * @return true iff tested Vertex is referenced by triangulation
 */
bool Delaunator::haveVertex(Vertex* v) const {
        bool have = false;
        for(unsigned int i = this->vertices.size() - 1; i >= 0 && not have; i--) 
                have = (this->vertices[i] == v);
        return have;
}




/**
 * @param v tested Vertex 
 * @return true iff tested Vertex is referenced by triangulation and is one of the four corner vertice
 */
bool Delaunator::isCornerVertex(Vertex* v) const {
#ifdef DEBUG
        assert(v != NULL);
#endif
        return v->getID() <= 4;
}




/**
 * An external Edge is an Edge that rely two corner Vertex.
 * @param e tested Edge
 * @return true iff tested Edge is referenced by triangulation and is one of the eight external Edge
 */
bool Delaunator::isExternalEdge(Edge *e) const {
#ifdef DEBUG
        assert(e != NULL);
#endif
        //return e->getID() <= 12;
        // an edge is external if in contact with a unvisible face. 
        return !e->leftFace()->isVisible() || !e->rightFace()->isVisible();
}




/**
 * @param c Coordinates of tested point
 * @return true if point is in-limit of this
 */
bool Delaunator::collideAt(Coordinates c) const {
        return !(c.x() < this->xmin || this->xmax < c.x() || c.y() < this->ymin || this->ymax < c.y());
}





/***************************************************
 * ITERATORS
 ***************************************************/
/**
 * Infinite iteration on all neighbour vertices of a given Vertex.
 * @param v the targeted Vertex
 * @return an IteratorVertexToNeighbourVertices that iter on neighbors of target
 */
IteratorVertexToNeighbourVertices Delaunator::getNeighbors(Vertex* v) {
        return IteratorVertexToNeighbourVertices(v);
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/
/**
 * @param target Coordinates that must be different of NULL.
 * @return address of Face of Delaunator that contains given coordinates, or NULL if error. If out of bounds, return the unvisible face that contain p
 */
Face* Delaunator::findContainerOf(Coordinates target) const {
// initialization
        Face* container = NULL;
        Edge *edge_cur = NULL;

#ifdef DEBUG
        assert(this->collideAt(target));
#endif

// choose the initial Edge ( >edge> )
        switch(this->finder_mode) {
                case Delaunator::FINDER_INITIAL_RANDOM:
                        edge_cur = this->edges[randN(this->edges.size())];
                        break;
                case Delaunator::FINDER_INITIAL_FIRST:
                        edge_cur = this->edges[0];
                        break;
                case Delaunator::FINDER_INITIAL_MIDDLE:
                        edge_cur = this->edges[this->edges.size() / 2];
                        break;
                case Delaunator::FINDER_INITIAL_LAST:
                default:
                        edge_cur = this->edges[this->edges.size() - 1];
                        break;
        }

// while face not found, search face ( >edge>, container> , >counter> )
#ifdef DEBUG
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
#ifdef DEBUG
                assert(edge_nxt != NULL); // there is always an edge that go on the right of the target
                assert(edge_nxt->originVertex() == edge_cur->originVertex());
#endif

                // Jump to next edge
                edge_cur->passing = true;
                edge_cur = edge_nxt->nextLeftEdge();
                if(counter_left >= 3)   container = edge_cur->leftFace();


#ifdef DEBUG
                assert((iter_count++) < (this->edges.size()*2+4));
#endif
        }
#ifdef DEBUG
        assert(container->collideAt(target));
#endif

// ending
        return container;
}






/**
 * Operate Delaunay Flip Algorithm on Face if necessary.
 * Recursiv call on new triangles created by flip.
 * @param f_ref a reference to a Face that can't be NULL and must be integrated in triangulation
 * @param ttl time-to-live, or number of recursiv call operated
 * @return true if modifications operate on tiangulation
 */
#ifdef DEBUG
bool Delaunator::applyDelaunayCondition(Face* f_ref, unsigned int ttl) {
        assert(f_ref != NULL && f_ref->getEdge() != NULL);
#else 
bool Delaunator::applyDelaunayCondition(Face* f_ref) {
#endif
// INITIALIZATION
        bool flip_done = true; // we are optimist.
        Edge* illegal_edge = NULL; // illegal edge that relie the illegal vertex and the f_ref's vertex.

// CONDITION: in circumcircle
        if(f_ref->isVisible()) {
                // FIRST NEIGHBOUR
                if(f_ref->circumcircleContainCoords(*f_ref->getEdge1()->nextRightEdge()->destinVertex())) {
                        illegal_edge = f_ref->getEdge1();

                // SECOND NEIGHBOUR
                } else if(f_ref->circumcircleContainCoords(*f_ref->getEdge2()->nextRightEdge()->destinVertex())) {
                        illegal_edge = f_ref->getEdge2();

                // THIRD NEIGHBOUR
                } else if(f_ref->circumcircleContainCoords(*f_ref->getEdge3()->nextRightEdge()->destinVertex())) {
                        illegal_edge = f_ref->getEdge3();

                // NO ILLEGAL SIDE
                } else  flip_done = false;
        } else flip_done = false;


// MODIFICATION OF FACES
        if(flip_done) {
                this->operateFlip(illegal_edge);
                // Recursiv call on the updated faces
#ifdef DEBUG
                // protection against infinite recursive call.
                if(ttl < this->faces.size() / 2) {
                        this->applyDelaunayCondition(illegal_edge->leftFace(), ttl+1);
                        this->applyDelaunayCondition(illegal_edge->rightFace(), ttl+1);
                } else {
                        logs("TTL %u reached !\n", ttl);
                }
#else
                this->applyDelaunayCondition(illegal_edge->leftFace());
                this->applyDelaunayCondition(illegal_edge->rightFace());
#endif
        }


// ENDING
        return flip_done;
}





/**
 * Operate the flip algorithm, as if the received Edge don't respect the Delaunay condition.
 * @param illegal_edge1 an Edge that is defined illegal
 */
void Delaunator::operateFlip(Edge* illegal_edge1) {
// DEDUCE SOME SHORTCUTS
        Edge *illegal_edge2 = illegal_edge1->oppositeEdge();
        Vertex *illegal_vertex1 = illegal_edge1->nextLeftEdge()->destinVertex();
        Vertex *illegal_vertex2 = illegal_edge1->nextRightEdge()->destinVertex();
        Face *face1 = illegal_edge2->rightFace();
        Face *face2 = illegal_edge2->leftFace();
        // previous and next edge in the cycle that form the Face.
        Edge* edge1_prev = illegal_edge1->nextLeftEdge();
        Edge* edge1_next = illegal_edge1->nextRightEdge()->oppositeEdge();
        Edge* edge2_prev = illegal_edge2->nextLeftEdge();
        Edge* edge2_next = illegal_edge2->nextRightEdge()->oppositeEdge();


#ifdef DEBUG
// TESTS
        assert(illegal_edge1->originVertex() != illegal_vertex1);
        assert(illegal_edge2->originVertex() != illegal_vertex1);
        assert(illegal_edge1->originVertex() != illegal_vertex2);
        assert(illegal_edge2->originVertex() != illegal_vertex2);
#endif

// MODIFICATIONS
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
        face1->setEdge(illegal_edge1);
        face2->setEdge(illegal_edge2);

#ifdef DEBUG
// TESTS
        assert(illegal_edge1->originVertex() == illegal_vertex1);
        assert(illegal_edge1->nextLeftEdge() == illegal_edge2->nextRightEdge()->nextRightEdge()->oppositeEdge());
        assert(illegal_edge1->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == illegal_edge1);
        assert(illegal_edge1->leftFace() == face1);
        assert(illegal_edge1->nextLeftEdge()->leftFace() == face1);
        assert(illegal_edge1->nextLeftEdge()->nextLeftEdge()->leftFace() == face1);

        assert(illegal_edge2->destinVertex() == illegal_vertex1);
        assert(illegal_edge2->nextLeftEdge()->nextLeftEdge()->nextLeftEdge() == illegal_edge2);
        assert(illegal_edge2->leftFace() == face2);
        assert(illegal_edge2->nextLeftEdge()->leftFace() == face2);
        assert(illegal_edge2->nextLeftEdge()->nextLeftEdge()->leftFace() == face2);

        this->DEBUG_tests();
#endif
#ifdef FOLLOW_SEARCH
        illegal_edge1->passing= false;
        illegal_edge2->passing= false;
#endif
}



