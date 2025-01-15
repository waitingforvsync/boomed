#ifndef BOOMED_WORLD_H_
#define BOOMED_WORLD_H_

#include "boomed/math/aabb2f.h"
#include "boomed/array.h"
#include "boomed/arena.h"

typedef struct vertex_t vertex_t;
typedef struct edge_t edge_t;
typedef struct contour_t contour_t;
typedef struct subzone_t subzone_t;
typedef struct zone_t zone_t;
typedef struct world_t world_t;
typedef uint16_t element_id_t;

#define ID_NONE (0xFFFFU)


// Define structs relating to the world.
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


// Representation of a vertex in the world.
// Vertices have a 2D integer position and maintain an array of the edge IDs which connect to them.
struct vertex_t {
    vec2i_t position;
    array_t(element_id_t, edge_ids);
};


// Representation of an edge in the world.
// Edges are defined by the two vertices they connect (this is an unordered set),
// and have an upper and lower colour for when walls need to be rendered between adjacent zones.
// They maintain the zone IDs either side of them, and the order is related to the vertex ID order:
// if we imagine a zone perimeter to be defined by edges in clockwise order, then the internal zone
// has the same index as the vertex id corresponding to the first vertex of the clockwise edge.
// The other zone index corresponds to the zone outside the perimeter (or none).
// Note this is reversed in the case of hole edges.
struct edge_t {
    element_id_t vertex_ids[2];
    element_id_t zone_ids[2];
    uint8_t upper_colour;
    uint8_t lower_colour;
};


// A contour is just an array of edge IDs.
// Contours are used to define zone perimeters and also any holes which may be present within a zone.
// A perimeter contour is defined in clockwise order, while a hole contour is defined in counter-clockwise order.
struct contour_t {
    array_t(element_id_t, edge_ids);
};


// A subzone is just an array of vertex IDs defined in a clockwise order.
struct subzone_t {
    array_t(element_id_t, vertex_ids);
};


// A zone is the irregular polygon formed by a closed contour of edges.
// It may optionally have any number of holes.
// It has a floor and ceiling height and corresponding colour.
// A zone is decomposed into a list of convex subzones.
// Zones maintain a hierarchy of which other zones exist within them and which zone they themselves are within.
struct zone_t {
    contour_t perimeter;
    array_t(contour_t, holes);
    array_t(subzone_t, subzones);
    array_t(element_id_t, inner_zone_ids);
    aabb2f_t aabb;
    element_id_t outer_zone_id;
    uint8_t floor_height;
    uint8_t ceiling_height;
    uint8_t floor_colour;
    uint8_t ceiling_colour;
};


// This is the world, composed of vertices, edges and zones.
// Here we maintain a number of arenas used for dynamically allocating these elements.
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

element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *edge_arena, arena_t scratch);
bool world_reindex_edge(world_t *world, element_id_t old_id, element_id_t new_id);
bool world_remove_last_edge(world_t *world);


element_id_t vertex_find_closest_to_point(const vertex_t vertices[], uint32_t num_vertices, vec2f_t point, float within);

aabb2f_t edge_get_aabb(const edge_t *edge, const vertex_t vertices[]);
element_id_t edge_find_closest_to_point(const edge_t edges[], uint32_t num_edges, const vertex_t vertices[], vec2f_t point, float within);

const element_id_t *contour_get_vertices(const contour_t *contour, const edge_t edges[], arena_t *arena);

aabb2f_t zone_get_aabb(const zone_t *zone, const edge_t edges[], const vertex_t vertices[]);



#endif // ifndef BOOMED_WORLD_H_
