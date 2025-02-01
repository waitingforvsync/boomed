#ifndef WORLD_ZONE_H_
#define WORLD_ZONE_H_

#include "math/aabb2f.h"
#include "world/element_id.h"
#include "world/contour.h"
#include "world/subzone.h"

typedef struct vertex_view_t vertex_view_t;
typedef struct edge_view_t edge_view_t;
typedef struct zone_t zone_t;
typedef struct arena_t arena_t;


// A zone is the irregular polygon formed by a closed contour of edges.
// It may optionally have any number of holes.
// It has a floor and ceiling height and corresponding colour.
// A zone is decomposed into a list of convex subzones.
// Zones maintain a hierarchy of which other zones exist within them and which zone they themselves are within.
struct zone_t {
    contour_t perimeter;
    contours_t holes;
    subzones_t subzones;
    element_ids_t inner_zone_ids;
    aabb2f_t aabb;
    element_id_t outer_zone_id;
    uint8_t floor_height;
    uint8_t ceiling_height;
    uint8_t floor_colour;
    uint8_t ceiling_colour;
};

// Define zone_t array type
#define TEMPLATE_NAME zones
#define TEMPLATE_TYPE zone_t
#include "templates/array.h.template"


void zone_build_subzones(zone_t *zone, vertices_view_t vertices, edges_view_t edges, arena_t *arena, arena_t scratch);

aabb2f_t zone_get_aabb(const zone_t *zone, vertices_view_t vertices, edges_view_t edges);


#endif // ifndef WORLD_ZONE_H_
