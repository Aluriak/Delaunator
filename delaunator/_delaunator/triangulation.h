#pragma once

typedef int32_t coord_t;


/**
 * Finder mode for when triangulation looking for the Face that contain Coordinates 
 *   (notabily for Vertex adding).
 * Random: random Edge in all existing in Triangulation instance
 * First: the first added Edge
 * Middle: the Edge in the middle of the Edge list
 * Last: the last added Edge (default value)
 */
enum VertexFinderMode {
        VERTEX_FINDER_MODE_RANDOM,
        VERTEX_FINDER_MODE_FIRST,
        VERTEX_FINDER_MODE_MIDDLE,
        VERTEX_FINDER_MODE_LAST
};



/*
 * For some explanations on quad-edge implementation :
 * http://totologic.blogspot.fr/2013/11/core-quad-edge-implementation-explained.html
*/


typedef struct Edge   Edge;
typedef struct Vertex Vertex;
typedef struct Face   Face;
typedef struct Coordinates   Coordinates;
typedef struct Triangulation Triangulation;


// API publique (exposée à Python)
Triangulation* tri_new(coord_t xmin, coord_t xmax, coord_t ymin, coord_t ymax);
void           tri_free(Triangulation* t);
Vertex*        tri_add_vertex(Triangulation* t, coord_t x, coord_t y);
Vertex*        tri_move_vertex(Triangulation* t, Vertex* v, coord_t nx, coord_t ny);
void           tri_del_vertex(Triangulation* t, Vertex* v);
// Pour le rendu : remplit un buffer de flottants (x1,y1,x2,y2,...)
int            tri_get_edges(Triangulation* t, coord_t* buf, uint64_t max_edges);

void           unit_tests(); // call that once for verify integrity of geometry fonctions
