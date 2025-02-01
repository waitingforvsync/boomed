#ifndef VERTEX_H_
#define VERTEX_H_

#include "math/vec2i.h"
#include "world/element_id.h"


typedef struct vertex_t vertex_t;

// Representation of a vertex in the world.
// Vertices have a 2D integer position and maintain an array of the edge IDs which connect to them.
struct vertex_t {
    vec2i_t position;
    element_ids_t edge_ids;
};

// Define vertex_t array type
#define TEMPLATE_NAME vertices
#define TEMPLATE_TYPE vertex_t
#include "templates/array.h.template"


vertex_t vertex_make(vec2i_t position, arena_t *ids_arena);

uint32_t vertex_get_connected_edge_index(const vertex_t *vertex, element_id_t edge_id);
element_id_t vertex_get_prev_connected_edge(const vertex_t *vertex, element_id_t edge_id);
element_id_t vertex_get_next_connected_edge(const vertex_t *vertex, element_id_t edge_id);



#endif // ifndef VERTEX_H_
