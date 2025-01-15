#include <assert.h>
#include "boomed/world/contour.h"
#include "boomed/world/edge.h"
#include "boomed/world/vertex.h"
#include "boomed/arena.h"


contour_t contour_make(const vertex_t *vertices, const edge_t *edges, element_id_t edge_id, element_id_t start_vertex_id, arena_t *arena) {
    assert(vertices);
    assert(edges);

    contour_t contour = {0};
    array_reserve(contour.edge_ids, arena, 256);

    element_id_t vertex_id = start_vertex_id;
    array_add(contour.edge_ids, arena, edge_id);

    while (!array_is_empty(contour.edge_ids)) {
        vertex_id = edge_get_other_vertex(&edges[edge_id], vertex_id);
        edge_id = vertex_get_next_edge(&vertices[vertex_id], edge_id);

        if (vertex_id == start_vertex_id && edge_id == contour.edge_ids[0]) {
            break;
        }

        if (array_get_last(contour.edge_ids) == edge_id) {
            array_pop(contour.edge_ids);
        }
        else {
            array_add(contour.edge_ids, arena, edge_id);
        }
    }

    return contour;
}


bool contour_is_valid(const contour_t *contour) {
    assert(contour);
    return !array_is_empty(contour->edge_ids);
}


element_id_t contour_get_start_vertex(const contour_t *contour, const edge_t *edges) {
    assert(contour);
    assert(edges);

    if (contour->edge_ids_num > 2) {
        const edge_t *start_edge = &edges[contour->edge_ids[0]];
        const edge_t *next_edge = &edges[contour->edge_ids[1]];

        if (start_edge->vertex_ids[0] == next_edge->vertex_ids[0] || start_edge->vertex_ids[0] == next_edge->vertex_ids[1]) {
            return start_edge->vertex_ids[1];
        }
        else if (start_edge->vertex_ids[1] == next_edge->vertex_ids[0] || start_edge->vertex_ids[1] == next_edge->vertex_ids[1]) {
            return start_edge->vertex_ids[0];
        }
    }
    return ID_NONE;
}


const element_id_t *contour_get_vertices(const contour_t *contour, const edge_t *edges, arena_t *arena) {
    assert(contour);
    assert(edges);
    assert(arena);

    element_id_t *vertex_ids = arena_new_n(element_id_t, contour->edge_ids_num, arena);
    element_id_t vertex_id = contour_get_start_vertex(contour, edges);
    for (uint32_t i = 0; i < contour->edge_ids_num; ++i) {
        vertex_ids[i] = vertex_id;
        vertex_id = edge_get_other_vertex(&edges[contour->edge_ids[i]], vertex_id);
    }
    return vertex_ids;
}


int32_t contour_get_winding(const contour_t *contour, const vertex_t *vertices, const edge_t *edges) {
    assert(contour);
    assert(vertices);
    assert(edges);

    if (!contour_is_valid(contour)) {
        return 0;
    }

    element_id_t v0_id = contour_get_start_vertex(contour, edges);
    assert(v0_id != ID_NONE);

    int32_t winding = 0;
    for (uint32_t i = 0; i < contour->edge_ids_num; ++i) {
        element_id_t v1_id = edge_get_other_vertex(&edges[contour->edge_ids[i]], v0_id);
        vec2i_t p0 = vertices[v0_id].position;
        vec2i_t p1 = vertices[v1_id].position;
        winding += (p0.x - p1.x) * (p0.y + p1.y);
        v0_id = v1_id;
    }
    return winding;
}


bool contour_is_perimeter(const contour_t *contour, const vertex_t *vertices, const edge_t *edges) {
    assert(contour);
    assert(vertices);
    assert(edges);
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) > 0;
}


bool contour_is_hole(const contour_t *contour, const vertex_t *vertices, const edge_t *edges) {
    assert(contour);
    assert(vertices);
    assert(edges);
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) < 0;
}
