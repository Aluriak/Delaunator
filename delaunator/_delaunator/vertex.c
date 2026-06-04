unsigned int vertex_last_id = 1;

Vertex* vtx_new(coord_t x, coord_t y, Edge* edge = NULL) {
    Vertex* t = malloc(sizeof *t);
    assert(t);
    t->x = x;
    t->y = y;
    t->id = t->last_id++;
    return t
}
