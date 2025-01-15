#ifndef BOOMED_WORLD_WORLD_H_
#define BOOMED_WORLD_WORLD_H_

#include "boomed/array.h"
#include "boomed/world/element_id.h"

typedef struct vertex_t vertex_t;
typedef struct edge_t edge_t;
typedef struct contour_t contour_t;
typedef struct zone_t zone_t;
typedef struct world_t world_t;
typedef struct vec2i_t vec2i_t;
typedef struct vec2f_t vec2f_t;


// Representation of the world, composed of vertices, edges and zones.
//
// We use a right-handed coordinate system, with X up and Y right (and thus Z away, if viewing the
// world from "above", so ceilings have a lower Z than floors)
//
//       X
//       |
//       +----Y
//
// The winding order of polygons is clockwise, i.e. going from the X axis to the Y axis.
// This lets us work with sensible maths (i.e. (1,0) ^ (0,1) = 1 for a clockwise turn),
// while allowing us to render zones from left to right.

struct world_t {
    array_t(vertex_t, vertices);
    array_t(edge_t, edges);
    array_t(zone_t, zones);
};


void world_init(world_t *world, arena_t *arena);
void world_reset(world_t *world);

element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *vertex_arena);
bool world_reindex_vertex(world_t *world, element_id_t old_id, element_id_t new_id);
void world_move_vertex(world_t *world, element_id_t vertex_id);
bool world_remove_last_vertex(world_t *world);

typedef struct add_edge_result_t add_edge_result_t;
struct add_edge_result_t {
    element_id_t new_edge_id;
    element_id_t new_zone_id;
    element_id_t split_zone_id;
};

element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *edge_arena, arena_t scratch);
bool world_reindex_edge(world_t *world, element_id_t old_id, element_id_t new_id);
bool world_remove_last_edge(world_t *world);

element_id_t world_add_zone(world_t *world, const contour_t *contour, arena_t *arena, arena_t scratch);

element_id_t world_find_vertex_closest_to_point(const world_t *world, vec2f_t point, float within);
element_id_t world_find_edge_closest_to_point(const world_t *world, vec2f_t point, float within);



#endif // ifndef BOOMED_WORLD_WORLD_H_
