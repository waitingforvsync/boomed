#include <assert.h>
#include "boomed/world/vertex.h"


uint32_t vertex_get_connected_edge_index(const vertex_t *vertex, element_id_t edge_id) {
    for (uint32_t i = 0; i < vertex->edge_ids_num; ++i) {
        if (vertex->edge_ids[i] == edge_id) {
            return i;
        }
    }
    return INDEX_NONE;
}


element_id_t vertex_get_prev_edge(const vertex_t *vertex, element_id_t edge_id) {
    uint32_t i = vertex_get_connected_edge_index(vertex, edge_id);
    assert(i != INDEX_NONE);
    return (i == 0) ? array_get_last(vertex->edge_ids) : vertex->edge_ids[i - 1];
}


element_id_t vertex_get_next_edge(const vertex_t *vertex, element_id_t edge_id) {
    uint32_t i = vertex_get_connected_edge_index(vertex, edge_id);
    assert(i != INDEX_NONE);
    return (i + 1 == vertex->edge_ids_num) ? array_get_first(vertex->edge_ids) : vertex->edge_ids[i + 1];
}
