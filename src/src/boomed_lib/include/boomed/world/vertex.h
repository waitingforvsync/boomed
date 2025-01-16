#ifndef BOOMED_WORLD_VERTEX_H_
#define BOOMED_WORLD_VERTEX_H_

#include "boomed/array.h"
#include "boomed/world/element_id.h"
#include "boomed/math/vec2i.h"


typedef struct vertex_t vertex_t;

// Representation of a vertex in the world.
// Vertices have a 2D integer position and maintain an array of the edge IDs which connect to them.
struct vertex_t {
    vec2i_t position;
    array_t(element_id_t, edge_ids);
};


vertex_t vertex_make(vec2i_t position, arena_t *ids_arena);

uint32_t vertex_get_connected_edge_index(const vertex_t *vertex, element_id_t edge_id);
element_id_t vertex_get_prev_edge(const vertex_t *vertex, element_id_t edge_id);
element_id_t vertex_get_next_edge(const vertex_t *vertex, element_id_t edge_id);



#endif // ifndef BOOMED_WORLD_VERTEX_H_
