#ifndef BOOMED_WORLD_ZONE_H_
#define BOOMED_WORLD_ZONE_H_

#include "boomed/array.h"
#include "boomed/math/aabb2f.h"
#include "boomed/world/element_id.h"
#include "boomed/world/contour.h"

typedef struct vertex_view_t vertex_view_t;
typedef struct edge_t edge_t;
typedef struct subzone_t subzone_t;
typedef struct zone_t zone_t;
typedef struct arena_t arena_t;


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


void zone_build_subzones(zone_t *zone, vertex_view_t vertices, const edge_t *edges, arena_t *arena, arena_t scratch);

aabb2f_t zone_get_aabb(const zone_t *zone, vertex_view_t vertices, const edge_t *edges);


#endif // ifndef BOOMED_WORLD_ZONE_H_
