#include "boomed/reflect.h"
#include "boomed/world/vertex.h"
#include "boomed/world/edge.h"
#include "boomed/world/contour.h"
#include "boomed/world/zone.h"
#include "boomed/world/world.h"


REFLECT_DEF_STRUCT(vec2i_t) {
    REFLECT(vec2i_t, x, int32_t),
    REFLECT(vec2i_t, y, int32_t)
};

REFLECT_DEF_STRUCT(vertex_t) {
    REFLECT_STRUCT(vertex_t, position, vec2i_t)
};

REFLECT_DEF_STRUCT(edge_t) {
    REFLECT(edge_t, vertex_ids, uint16_t),
    REFLECT(edge_t, upper_colour, uint8_t),
    REFLECT(edge_t, lower_colour, uint8_t)
};

REFLECT_DEF_STRUCT(contour_t) {
    REFLECT_ARRAY(contour_t, edge_ids, uint16_t)
};

REFLECT_DEF_STRUCT(zone_t) {
    REFLECT_STRUCT(zone_t, perimeter, contour_t),
    REFLECT_STRUCT_ARRAY(zone_t, holes, contour_t),
    REFLECT(zone_t, floor_height, uint8_t),
    REFLECT(zone_t, ceiling_height, uint8_t),
    REFLECT(zone_t, floor_colour, uint8_t),
    REFLECT(zone_t, ceiling_colour, uint8_t)
};

REFLECT_DEF_STRUCT(world_t) {
    REFLECT_STRUCT_ARRAY(world_t, vertices, vertex_t),
    REFLECT_STRUCT_ARRAY(world_t, edges, edge_t),
    REFLECT_STRUCT_ARRAY(world_t, zones, zone_t)
};
