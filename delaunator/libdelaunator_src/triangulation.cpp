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
Triangulation::Triangulation(const float xmin, const float xmax, 
                             const float ymin, const float ymax, 
                             const VertexFinderMode finder_mode) : 
                                   xmin(xmin), xmax(xmax), 
                                   ymin(ymin), ymax(ymax) {
#ifdef DEBUG
        assert(xmin < xmax);
        assert(ymin < ymax);
#endif
// Initialize some values
        this->setFinderMode(finder_mode);
// Creation of primitive mesh, with four points.
        Vertex* vnw = new Vertex(xmin-1, ymin-1); // NORTH-WEST
        Vertex* vne = new Vertex(xmax+1, ymin-1); // NORTH-EAST
        Vertex* vse = new Vertex(xmax+1, ymax+1); // SOUTH-EAST
        Vertex* vsw = new Vertex(xmin-1, ymax+1); // SOUTH-WEST
        this->vertices.push_back(vnw); 
        this->vertices.push_back(vne); 
        this->vertices.push_back(vse); 
        this->vertices.push_back(vsw); 

// Creation of Edges that rely vertices.
        Edge* eNW2NE = new Edge(vnw) ;
        Edge* eNW2SE = new Edge(vnw) ;
        Edge* eNW2SW = new Edge(vnw) ;
        this->edges.push_back(eNW2NE);
        this->edges.push_back(eNW2SE);
        this->edges.push_back(eNW2SW);

        Edge* eNE2NW = new Edge(vne) ;
        Edge* eNE2SE = new Edge(vne) ;
        Edge* eNE2SW = new Edge(vne) ;
        this->edges.push_back(eNE2NW);
        this->edges.push_back(eNE2SE);
        this->edges.push_back(eNE2SW);

        Edge* eSE2NW = new Edge(vse) ;
        Edge* eSE2NE = new Edge(vse) ;
        Edge* eSE2SW = new Edge(vse) ;
        this->edges.push_back(eSE2NW);
        this->edges.push_back(eSE2NE);
        this->edges.push_back(eSE2SW);

        Edge* eSW2NW = new Edge(vsw) ;
        Edge* eSW2NE = new Edge(vsw) ;
        Edge* eSW2SE = new Edge(vsw) ;
        this->edges.push_back(eSW2NW);
        this->edges.push_back(eSW2NE);
        this->edges.push_back(eSW2SE);

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
        assert(eNW2NE->oppositeEdge() == eNE2NW);
        assert(eSE2SW->oppositeEdge() == eSW2SE);
        geometry::unit_tests();
#endif

// Creation of the four faces. (two external faces exist, but are not used, for simplify treatments)
        this->faces.push_back(new Face(eSW2NE));
        this->faces.push_back(new Face(eNE2SW));
        this->faces.push_back(new Face(eSW2NW, false)); // extern face, describe outer world
        this->faces.push_back(new Face(eNE2SE, false)); // extern face, describe outer world

#ifdef DEBUG
        assert(eSW2NE->leftFace() != eNE2SW->leftFace()); // faces must be all different
        assert(eNE2SW->leftFace() != eSE2SW->leftFace()); // but all inequality are not tested
        assert(eSE2SW->leftFace() != eNE2SE->leftFace()); // these assert will be silent
        assert(eNE2SE->leftFace() != eSW2NE->leftFace()); // until a very big bug is detected
        assert(eSW2NE->leftFace() != eSE2SW->leftFace());  
        assert(eSW2NE->leftFace() != eNE2SE->leftFace());  
        this->DEBUG_tests();
#endif
}



/**
 * Frees all Faces, Vertices and Edges.
 */
Triangulation::~Triangulation() {
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
 * @param p Coordinates where Vertex will be add
 * @param first initial Edge use by finder, in place of the one taked according to FinderMode
 * @return address of that point in Mesh, or NULL if not found.
 * @note if out of bounds, a correction is applied.
 */
Vertex* Triangulation::addVertexAt(Coordinates p, Edge* first) {
// initialization
        Face* container = NULL; // container of p
        Vertex* new_vertex = NULL; // returned vertex
        p = this->coordinateCorrection(p);
        if(this->collideAt(p)) {

// find container of vertex ( container>, >p )
#ifdef FOLLOW_SEARCH
        for(IteratorOnAllEdges it = this->iterAllEdges(); it != it.end(); it++) {
                (*it)->passing = false;
        }
#endif
        container = this->findContainerOf(p, first);
        if(container != NULL) {
        // verify if new Vertex is not confound with another
                Vertex* v1 = container->getP1();
                Vertex* v2 = container->getP2();
                Vertex* v3 = container->getP3();
                if(p == *v1)      new_vertex = v1; // don't need to add a Vertex if a Vertex is already here !
                else if(p == *v2) new_vertex = v2; // so we return existing Vertex to user.
                else if(p == *v3) new_vertex = v3;
        // if a Vertex must be creat
                if(new_vertex == NULL) {
        
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
                        new_vertex = this->vertices.back();
                        Vertex* v0 = new_vertex;
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
                } // if(new_vertex == NULL)
                //else logs("Add a Vertex on existing one; no Vertex created\n");
// ending
        } // if(container != NULL)
        } // if(this->collideAt(p))
        return new_vertex;
}







/**
 * Move given vertex by given vector values.
 * Modify the mesh in consequence.
 * Vertex can't pass over the mesh bounds.
 * @param mv_vrtx the Vertex to move
 * @param vec_x move in x axis
 * @param vec_y move in y axis
 * @return address of the Vertex, or of a new Vertex if given was freed
 * @note given Vertex can be deleted, but data is keeped in returned Vertex.
 */
Vertex* Triangulation::moveVertex(Vertex* mv_vrtx, float x, float y) { 
        return this->moveVertexTo(mv_vrtx, Coordinates(mv_vrtx->x()+x, mv_vrtx->y()+y)); 
}



/**
 * Move given vertex at given Coordinates.
 * Modify the mesh in consequence.
 * Vertex can't pass over the mesh bounds.
 * @param mv_vrtx the Vertex to move
 * @param new_position of the Vertex
 * @return address of the Vertex, or of a new Vertex if given was freed
 * @note given Vertex can be deleted, but data is keeped in returned Vertex.
 */
Vertex* Triangulation::moveVertexTo(Vertex* mv_vrtx, Coordinates new_position) {
        // ALGORITHM:
        //      - detect collision with limiter edges/Vertex
        //              - if edge is too short to be divide:
        //                      - create new vertex, and delete mv_vrtx
        //              - else:
        //                      - move to middle of the edge, and recursiv recall
        //
// INIT
        Edge* col_edge = NULL; // collision edge
        Edge* cur_edge = NULL; // currently studied edge
        Edge* lmt_edge = NULL; // limiter edge accessed from cur_edge

        if((*mv_vrtx) != new_position) {
// LIMIT MOVE
        //logs("LIMIT MOVE\n");
        new_position = this->coordinateCorrection(new_position);

// FIND COLLISION WITH A LIMITER EDGES 
        //logs("FIND COLLISION WITH A LIMITER EDGES\n");
        // limiter edges are the next left edges of all edge that have mv_vrtx as origin.
        cur_edge = mv_vrtx->getEdge();
        do {
                cur_edge = cur_edge->rotLeftEdge();
                lmt_edge = cur_edge->nextLeftEdge();
                // if collision, col_edge is finded !
                if(geometry::collisionBetweenSegmentAndSegment(
                                        *lmt_edge->originVertex(), 
                                        *lmt_edge->destinVertex(),
                                        *mv_vrtx, new_position)) {
                        col_edge = lmt_edge;
                }
        } while(col_edge == NULL && cur_edge != mv_vrtx->getEdge());

        // if targeted position is exactly a Vertex, next algorithm step need to manage that.
        // Help it: garantee that the targeted Vertex is the origin vertex of lmt_edge
        // For do that, just get opposite edge of lmt_edge if lmt_edge's destination vertex is
        //  at new_position
        if(col_edge != NULL && *col_edge->destinVertex() == new_position) {
                col_edge = col_edge->oppositeEdge();
#ifdef DEBUG
                assert(*col_edge->originVertex() == new_position);
#endif
        }

// MOVE MV_VRTX TO NEW LOCATION
        //logs("MOVE MV_VRTX TO NEW LOCATION\n");
        if(col_edge == NULL) {
                // next step is the end
                this->moveVertex_pure(mv_vrtx, new_position);

        } else if(*col_edge->originVertex() == new_position) { // if target place is already habited
#ifdef DEBUG
                assert(mv_vrtx != col_edge->originVertex());
                assert(not col_edge->originVertex()->isACorner());
#endif
                // give all to this existant Vertex
                mv_vrtx->giveVirtualVerticesTo(col_edge->originVertex());
#ifdef DEBUG
                assert(mv_vrtx->getObjectCount() == 0);
#endif
                // del mv_vrtx, because we don't need it anymore
                this->delVertex(mv_vrtx);
                // the mv_vrtx is now the existant one (and don't need to be moved anymore)
                mv_vrtx = col_edge->originVertex();

        } else if(col_edge->length() > 2*EPSILON) { // must be dividable
                // get current middle of collision edge as the next step
                Coordinates next_step = col_edge->middle();
                // operate flip on collision edge (can break Delaunay condition)
                this->operateFlip(col_edge);
                // go in the next step (Delaunay condition repaired)
                this->moveVertex_pure(mv_vrtx, next_step);
                // recursiv call, lets go to the next step !
                mv_vrtx = this->moveVertexTo(mv_vrtx, new_position);

        } else { // hard way
                // add new vertex
                Vertex* new_vrtx = this->addVertexAt(new_position, col_edge);
                // give all VirtualVertices to it
                mv_vrtx->giveVirtualVerticesTo(new_vrtx);
                // del the asked to move Vertex
                this->delVertex(mv_vrtx);
                // and finally break recursiv recall
                mv_vrtx = new_vrtx; 
                // no anymore move required !
        }
        
// ENDING
        }
        return mv_vrtx;
}







/**
 * @return vertex found at given coords, around precision. or NULL iff no vertex found.
 * @note never return the corners Vertex
 */
Vertex* Triangulation::vertexAt(float x, float y, float precision) const {
        Vertex *target = NULL, *current = NULL;
        for(IteratorOnVertices_read it = this->iterVertices_read(); target == NULL && it != it.end(); it++) {
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
void Triangulation::delVertex(Vertex* del_vrtx) {
        // ALGORITHM:
        // for all direct neighbour:
        //      if the triangle formed by three consecutiv neighbors DOESN'T contain del_vrtx:
        //              TODO
// INIT
#ifdef DEBUG
        assert(del_vrtx != NULL);
        assert(this->have(del_vrtx));
        assert(!del_vrtx->isACorner());
#endif
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
        // When we can't do something, it's when the del_vrtx is linked to three or four points exactly.
        // The end of local simplification is when no modification is done.
        // At this time of the algo, Delaunay condition is breaked, and suppression of the point
        // is easy. (point will be in a triangle, or on an edge exactly)
        unsigned int prev_size = 3;
        while(prev_size != nei_vrtx.size()) {
                prev_size = nei_vrtx.size();
                // For each neighbour
                for(unsigned int target = 0; target < nei_edge.size(); target++) {
                        // take tree consecutivs neighbors indexes
                        unsigned int id1 = target;
                        unsigned int id2 = (target+1) % nei_dist.size();
                        unsigned int id3 = (target+2) % nei_dist.size();
                        // collision between del_vrtx and triangle formed by neighbors
                        bool del_vrtx_not_in_triangle = not geometry::pointInTriangle(
                                        *nei_vrtx[id1], *nei_vrtx[id2], *nei_vrtx[id3], *del_vrtx
                        );
                        // if middle point is farthest than at least one of the two others from del_vrtx
                        //      AND del_vrtx is not in triangle formed by the tree neighbors
                        if(del_vrtx_not_in_triangle &&
                                (nei_dist[id2] >= nei_dist[id1] || nei_dist[id2] >= nei_dist[id3])) {
                                // these tree points will be a triangle !
                                // so, operate flip on the middle edge
                                this->operateFlip(nei_edge[id2]);
                                // remove middle vertex (id2) from neighbors lists
                                nei_edge.erase(nei_edge.begin() + id2);
                                nei_dist.erase(nei_dist.begin() + id2);
                                nei_vrtx.erase(nei_vrtx.begin() + id2);
                                // The modified faces must be referenced : maybe it break the Delaunay condition
                                modified_faces.push_back(nei_edge[id1]->nextLeftEdge()->rightFace());
                        }
                }
        }
        //logs("\nNeighbors reduced to %i\n", nei_vrtx.size());
#ifdef DEBUG
                // Some tests
                // del_vrtx is in a triangle or in a square and aligned with at least two neighbors
                assert(nei_vrtx.size() == 4 || nei_vrtx.size() == 3);
                this->DEBUG_tests();
#endif
        
// DELETE POINT FROM TRIANGLE CONTAINER
        if(nei_vrtx.size() == 3) { // triangle
                // Container of del_vrtx is composed by the three neighbors contains in nei_vrtx
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
                assert(edge3  != edge1  && edge1  != edge2  && edge2  != edge3 );
                assert(side3l != side1l && side1l != side2l && side2l != side3l);
                assert(f1 != f3 && f3 != f2 && f2 != f1);
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

// DELETE POINT FROM SQUARE CONTAINER
        } else if(nei_vrtx.size() == 4) { // in a square, aligned with two neighbors or more
                // Two triangles contains del_vrtx; delete it will be a bit more complex.
                // edge{1,2,3,4} leads to each Vertex of the square.
                // There is 4 triangles, only two will remain.
                // edge1 will be used for link vertices 2 and 4.
                Edge *edge1 = nei_edge.front();
                Edge *edge2 = edge1->rotLeftEdge();
                Edge *edge3 = edge2->rotLeftEdge();
                Edge *edge4 = edge3->rotLeftEdge();
                // vertex that will be linked by edge1 and its opposite
                Vertex* vertex2 = edge2->destinVertex();
                Vertex* vertex4 = edge4->destinVertex();
                // f2, f3 and f4 will be deleted. f1 and f2 are the future Faces of final triangles.
                Face* face1 = edge1->leftFace();
                Face* face2 = edge2->leftFace();
                Face* face3 = edge3->leftFace();
                Face* face4 = edge4->leftFace();
                // sides are the edge that are the sides of the square of faces f1 and f2.
                Edge *side1 = edge1->nextLeftEdge();
                Edge *side2 = edge2->nextLeftEdge();
                Edge *side3 = edge3->nextLeftEdge();
                Edge *side4 = edge4->nextLeftEdge();
#ifdef DEBUG
                assert(edge1 != edge2 && edge1 != edge3 && edge1 != edge4);
                assert(edge2 != edge3 && edge2 != edge4 && edge3 != edge4);
                assert(side1 != side2 && side1 != side3 && side1 != side4);
                assert(side2 != side3 && side2 != side4 && side3 != side4);
                assert(face1 != face2 && face1 != face3 && face1 != face4);
                assert(face2 != face3 && face2 != face4 && face3 != face4);
#endif
                // Origin Vertices must refers sides, no edge1, edge2 or edge3
                side1->originVertex()->setEdge(side1);
                side2->originVertex()->setEdge(side2);
                side3->originVertex()->setEdge(side3);
                side4->originVertex()->setEdge(side4);
                // Set next left edge
                // side1 -> edge1 -> side4 == one triangle
                side1->setNextLeftEdge(edge1); 
                edge1->setNextLeftEdge(side4); 
                side4->setNextLeftEdge(side1); 
                // side2 -> side3 -> edge1->oppositeEdge() == second trianglev
                side2->setNextLeftEdge(side3); 
                side3->setNextLeftEdge(edge1->oppositeEdge()); 
                edge1->oppositeEdge()->setNextLeftEdge(side2);
                // Set face's Edge reference to a valide side, and reference it as a modified face
                face1->setEdge(side1);
                face2->setEdge(side2);
                modified_faces.push_back(face1); // only one face is enough
                // Set vertices 2 and 4 edge as edge1 and its opposite.
                edge1->setOriginVertex(vertex2);
                edge1->oppositeEdge()->setOriginVertex(vertex4);
                // Delete unwanted faces, edges, and finally del_vrtx
                this->removeEdgeFromEdges(edge2->oppositeEdge());
                this->removeEdgeFromEdges(edge3->oppositeEdge());
                this->removeEdgeFromEdges(edge4->oppositeEdge());
                this->removeEdgeFromEdges(edge2);
                this->removeEdgeFromEdges(edge3);
                this->removeEdgeFromEdges(edge4);
                this->removeFaceFromFaces(face3);
                this->removeFaceFromFaces(face4);
                this->removeVertexFromVertices(del_vrtx);
        }

// RESTORE DELAUNAY CONDITION
        // Delaunay condition was break. It's time to restore it.
        for(Face* face : modified_faces) 
                this->applyDelaunayCondition(face);

// END
#ifdef DEBUG
        this->DEBUG_tests();
#endif
}




/**
 * Merge two Vertex by destroying the second one
 * @param v the Vertex
 * @param v_destroyed the Vertex that will be destroyed
 * @note all iterators and pointers on v_destroyed will be destroyed
 */
void Triangulation::mergeVertex(Vertex* v, Vertex* v_destroyed) {
#if DEBUG
        assert(v->isNeighbourOf(v_destroyed));
#endif
        // Transmission of objects of v_destroyed to v
        v_destroyed->giveVirtualVerticesTo(v);
        // Deletion of v_destroyed
        this->delVertex(v_destroyed);
}





/**
 * Correct Coordinates
 * @param c Coordinates 
 * @return Coordinates that are equal to c, or, if c is out of bounds, a projection of c on this.
 */
Coordinates Triangulation::coordinateCorrection(Coordinates c) const {
        if(c.x() < this->xmin)  c.setX(this->xmin);
        if(c.x() > this->xmax)  c.setX(this->xmax);
        if(c.y() < this->ymin)  c.setY(this->ymin);
        if(c.y() > this->ymax)  c.setY(this->ymax);
        return c;
}





#ifdef DEBUG // some tests with assertions
/*
 * DEBUG TESTS.
 */
void Triangulation::DEBUG_tests() const {
        for(auto it = this->edges.cbegin(); it != this->edges.cend(); it++) {
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
        for(auto it = this->faces.cbegin(); it != this->faces.cend(); it++) {
                assert((*it)->getEdge() != NULL);
        }
        for(auto it = this->vertices.cbegin(); it != this->vertices.cend(); it++) {
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
 * @return VertexFinderMode value, that described method used by this instance.
 */
VertexFinderMode Triangulation::getFinderMode() const { 
        VertexFinderMode finder_mode = VERTEX_FINDER_MODE_RANDOM; 
        // testing pointer with different values it can took is necessary for find the mode
        if(this->finderInitialEdge == &Triangulation::finderInitial_middle)
                finder_mode = VERTEX_FINDER_MODE_MIDDLE;
        else if(this->finderInitialEdge == &Triangulation::finderInitial_first)
                finder_mode = VERTEX_FINDER_MODE_FIRST;
        else if(this->finderInitialEdge == &Triangulation::finderInitial_last)
                finder_mode = VERTEX_FINDER_MODE_LAST;
        return finder_mode;
}

/**
 * @param m VertexFinderMode value, that tell which Edge this instance will used as first Edge for search
 */
void Triangulation::setFinderMode(VertexFinderMode m) { 
        switch(m) {
                case VERTEX_FINDER_MODE_RANDOM:
                        this->finderInitialEdge = &Triangulation::finderInitial_random;
                        break;
                case VERTEX_FINDER_MODE_FIRST:
                        this->finderInitialEdge = &Triangulation::finderInitial_first;
                        break;
                case VERTEX_FINDER_MODE_MIDDLE:
                        this->finderInitialEdge = &Triangulation::finderInitial_middle;
                        break;
                case VERTEX_FINDER_MODE_LAST:
                default:
                        this->finderInitialEdge = &Triangulation::finderInitial_last;
                        break;
        }
}







/***************************************************
 * PREDICATS
 ***************************************************/
/**
 * @param v tested Vertex 
 * @return true iff tested Vertex is referenced by triangulation
 */
bool Triangulation::have(Vertex* v) const {
        return std::find(this->vertices.begin(), this->vertices.end(), v) != this->vertices.end();
}




/**
 * @param c Coordinates of tested point
 * @return true if point is in-limit of this
 */
bool Triangulation::collideAt(Coordinates c) const {
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
IteratorVertexToNeighbourVertices Triangulation::getNeighbors(Vertex* v) {
        return IteratorVertexToNeighbourVertices(v);
}





/***************************************************
 * PRIVATE METHODS
 ***************************************************/
/**
 * @param target Coordinates that must be valid (no out of bounds).
 * @param edge_cur the first Vertex used for research. if NULL, first will be set accordins to FinderMode.
 * @return address of Face of Triangulation that contains given coordinates, or NULL if error. If out of bounds, return the unvisible face that contain p
 */
Face* Triangulation::findContainerOf(Coordinates target, Edge* edge_cur) const {
// initialization
        Face* container = NULL;

#ifdef DEBUG
        assert(this->collideAt(target));
#endif

// choose the initial Edge ( >edge> )
        if(edge_cur == NULL) {
                edge_cur = (this->*finderInitialEdge)();
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
#ifdef FOLLOW_SEARCH
                edge_cur->passing = true;
#endif
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
bool Triangulation::applyDelaunayCondition(Face* f_ref, unsigned int ttl) {
        assert(f_ref != NULL && f_ref->getEdge() != NULL);
#else 
bool Triangulation::applyDelaunayCondition(Face* f_ref) {
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
void Triangulation::operateFlip(Edge* illegal_edge1) {
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



