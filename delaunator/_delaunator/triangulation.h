#pragma once

typedef int32_t coord_t;


/**
 * Finder mode for when triangulation looking for the Face that contain Coordinates 
 *   (notabily for Vertex adding).
 * Random: random Edge in all existing in Triangulation instance
 * First: the first added Edge
 * Middle: the Edge in the middle of the Edge list
 * Last: the last added Edge (default value)
 * @note Last mode is certainly the better, especially in case where added Vertex are close to previous one
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
typedef struct Triangulation Triangulation;

struct Edge {
    Vertex* origin;
    Edge*   opposite;
    Edge*   next_left;
    Face*   left_face;
    int     visible;
    unsigned int id;
};

struct Vertex {
    float x, y;
    Edge* edge;       // une arête sortante
    unsigned int id;
};

struct Face {
    Edge* edge;
    int   visible;
    float cx, cy;         // centroïde
    float ccx, ccy;       // circoncentre
    unsigned int id;
};

struct Triangulation {
    float xmin, xmax, ymin, ymax;

    Vertex** vertices;  int n_vertices, cap_vertices;
    Edge**   edges;     int n_edges,    cap_edges;
    Face**   faces;     int n_faces,    cap_faces;
};

// API publique (exposée à Python)
Triangulation* tri_new(float xmin, float xmax, float ymin, float ymax);
void           tri_free(Triangulation* t);
Vertex*        tri_add_vertex(Triangulation* t, float x, float y);
Vertex*        tri_move_vertex(Triangulation* t, Vertex* v, float nx, float ny);
void           tri_del_vertex(Triangulation* t, Vertex* v);
// Pour le rendu : remplit un buffer de flottants (x1,y1,x2,y2,...)
int            tri_get_edges(Triangulation* t, float* buf, int max_edges);

void           unit_tests(); // call that once for verify integrity of geometry fonctions
