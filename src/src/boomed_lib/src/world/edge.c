#include "boomed/world/edge.h"
#include "boomed/world/vertex.h"
#include "boomed/math/aabb2f.h"


void edge_set_zone_by_vertex(edge_t *edge, element_id_t vertex_id, element_id_t zone_id) {
    if (edge->vertex_ids[0] == vertex_id) {
        edge->zone_ids[0] = zone_id;
    }
    else if (edge->vertex_ids[1] == vertex_id) {
        edge->zone_ids[1] = zone_id;
    }
}


element_id_t edge_get_zone_by_vertex(const edge_t *edge, element_id_t vertex_id) {
    assert(edge);
    assert(vertex_id != ID_NONE);

    return (edge->vertex_ids[0] == vertex_id) ? edge->zone_ids[0] :
           (edge->vertex_ids[1] == vertex_id) ? edge->zone_ids[1] :
           ID_NONE;
}


element_id_t edge_get_other_vertex(const edge_t *edge, element_id_t vertex_id) {
    assert(edge);
    assert(vertex_id != ID_NONE);

    return (edge->vertex_ids[0] == vertex_id) ? edge->vertex_ids[1] :
           (edge->vertex_ids[1] == vertex_id) ? edge->vertex_ids[0] :
           ID_NONE;
}


aabb2f_t edge_get_aabb(const edge_t *edge, vertices_view_t vertices) {
    assert(edge);
    
    vec2f_t world_start_pos = vec2f_make_from_vec2i(vertices_view_get(vertices, edge->vertex_ids[0]).position);
    vec2f_t world_end_pos   = vec2f_make_from_vec2i(vertices_view_get(vertices, edge->vertex_ids[1]).position);
    return aabb2f_make(world_start_pos, world_end_pos);
}
