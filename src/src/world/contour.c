#include "base/arena.h"
#include "base/defines.h"
#include "world/contour.h"
#include "world/edge.h"
#include "world/vertex.h"


contour_t contour_make(vertices_view_t vertices, edges_view_t edges, element_id_t edge_id, element_id_t start_vertex_id, arena_t *arena) {
    assert(arena);
    contour_t contour = {
        .edge_ids = element_ids_make(arena, 256)
    };

    element_id_t vertex_id = start_vertex_id;
    element_ids_add(&contour.edge_ids, arena, edge_id);

    while (!element_ids_is_empty(&contour.edge_ids)) {
        vertex_id = edge_get_other_vertex(edges_view_get_ptr(edges, edge_id), vertex_id);
        const vertex_t *vertex = vertices_view_get_ptr(vertices, vertex_id);
        edge_id = vertex_get_next_edge(vertex, edge_id);

        if (vertex_id == start_vertex_id && edge_id == element_ids_get(&contour.edge_ids, 0)) {
            break;
        }

        if (element_ids_get_last(&contour.edge_ids) == edge_id) {
            element_ids_pop(&contour.edge_ids);
        }
        else {
            element_ids_add(&contour.edge_ids, arena, edge_id);
        }
    }

    return contour;
}


contour_t contour_make_copy(const contour_t *contour_to_copy, arena_t *arena) {
    assert(contour_to_copy);
    assert(arena);

    contour_t contour = {
        .edge_ids = element_ids_make_copy(contour_to_copy->edge_ids.view, arena, 32)
    };

    return contour;
}


bool contour_is_valid(const contour_t *contour) {
    assert(contour);
    return !element_ids_is_empty(&contour->edge_ids);
}


element_id_t contour_get_start_vertex(const contour_t *contour, edges_view_t edges) {
    assert(contour);

    if (contour->edge_ids.num > 2) {
        const edge_t *start_edge = edges_view_get_ptr(edges, element_ids_get(&contour->edge_ids, 0));
        const edge_t *next_edge  = edges_view_get_ptr(edges, element_ids_get(&contour->edge_ids, 1));

        if (start_edge->vertex_ids[0] == next_edge->vertex_ids[0] || start_edge->vertex_ids[0] == next_edge->vertex_ids[1]) {
            return start_edge->vertex_ids[1];
        }
        else if (start_edge->vertex_ids[1] == next_edge->vertex_ids[0] || start_edge->vertex_ids[1] == next_edge->vertex_ids[1]) {
            return start_edge->vertex_ids[0];
        }
    }
    return ID_NONE;
}


element_ids_view_t contour_get_vertices(const contour_t *contour, edges_view_t edges, arena_t *arena) {
    assert(contour);
    assert(arena);

    element_ids_view_t edge_ids = contour->edge_ids.view;
    element_ids_slice_t vertex_ids = arena_new_slice(element_id_t, edge_ids.num, arena);
    element_id_t vertex_id = contour_get_start_vertex(contour, edges);
    for (uint32_t i = 0; i < edge_ids.num; ++i) {
        element_ids_slice_set(vertex_ids, i, vertex_id);
        vertex_id = edge_get_other_vertex(
            edges_view_get_ptr(edges, element_ids_view_get(edge_ids, i)),
            vertex_id
        );
    }
    return vertex_ids.view;
}


int32_t contour_get_winding(const contour_t *contour, vertices_view_t vertices, edges_view_t edges) {
    assert(contour);

    if (!contour_is_valid(contour)) {
        return 0;
    }

    element_id_t v0_id = contour_get_start_vertex(contour, edges);
    assert(v0_id != ID_NONE);

    int32_t winding = 0;
    for (uint32_t i = 0; i < contour->edge_ids.num; ++i) {
        element_id_t v1_id = edge_get_other_vertex(
            edges_view_get_ptr(edges, element_ids_get(&contour->edge_ids, i)),
            v0_id
        );
        vec2i_t p0 = vertices_view_get(vertices, v0_id).position;
        vec2i_t p1 = vertices_view_get(vertices, v1_id).position;
        winding += (p0.x - p1.x) * (p0.y + p1.y);
        v0_id = v1_id;
    }
    return winding;
}


bool contour_is_perimeter(const contour_t *contour, vertices_view_t vertices, edges_view_t edges) {
    assert(contour);
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) > 0;
}


bool contour_is_hole(const contour_t *contour, vertices_view_t vertices, edges_view_t edges) {
    assert(contour);
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) < 0;
}
