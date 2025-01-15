#include "boomed/world/edge.h"
#include "boomed/world/vertex.h"
#include "boomed/math/aabb2f.h"


element_id_t edge_get_other_vertex(const edge_t *edge, element_id_t vertex_id) {
    assert(edge);
    assert(vertex_id != ID_NONE);

    if (vertex_id == edge->vertex_ids[0]) {
        return edge->vertex_ids[1];
    }
    else if (vertex_id == edge->vertex_ids[1]) {
        return edge->vertex_ids[0];
    }
    return ID_NONE;
}


aabb2f_t edge_get_aabb(const edge_t *edge, const vertex_t *vertices) {
    assert(edge);
    assert(vertices);
    
    vec2f_t world_start_pos = vec2f_make_from_vec2i(vertices[edge->vertex_ids[0]].position);
    vec2f_t world_end_pos   = vec2f_make_from_vec2i(vertices[edge->vertex_ids[1]].position);
    return aabb2f_make(world_start_pos, world_end_pos);
}
