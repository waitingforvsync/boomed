#include "base/defines.h"
#include "world/zone.h"
#include "world/vertex.h"
#include "world/edge.h"
#include "world/subzone.h"


void zone_build_subzones(zone_t *zone, vertices_view_t vertices, edges_view_t edges, arena_t *arena, arena_t scratch) {
    assert(zone);

    const element_id_t *vertex_ids = contour_get_vertices(&zone->perimeter, edges, &scratch);
    uint32_t num_vertices = zone->perimeter.edge_ids_num;

    subzones_reset(&zone->subzones);
    uint32_t i = subzones_push(&zone->subzones, arena);

    subzone_t *subzone = subzones_get_ptr(&zone->subzones, i);
    array_init_reserve(subzone->vertex_ids, arena, 32);
    array_resize(subzone->vertex_ids, arena, num_vertices);

    for (uint32_t j = 0; j < num_vertices; ++j) {
        subzone->vertex_ids[j] = vertex_ids[j];
    }
}


aabb2f_t zone_get_aabb(const zone_t *zone, vertices_view_t vertices, edges_view_t edges) {
    assert(zone);
    
    const element_id_t *edge_ids = zone->perimeter.edge_ids;

    aabb2f_t aabb = edge_get_aabb(edges_view_get_ptr(edges, edge_ids[0]), vertices);
    for (uint32_t i = 1; i < zone->perimeter.edge_ids_num; ++i) {
        aabb = aabb2f_union(aabb, edge_get_aabb(edges_view_get_ptr(edges, edge_ids[i]), vertices));
    }
    return aabb;
}
