#include "boomed/world.h"
#include "boomed/math/aabb2f.h"
#include "boomed/math/vec2f.h"


void world_init(world_t *world, arena_t *arena) {
    array_init_reserve(world->vertices, arena, 8192);
    array_init_reserve(world->edges, arena, 8192);
    array_init_reserve(world->zones, arena, 1024);
}


void world_reset(world_t *world) {
    array_reset(world->vertices);
    array_reset(world->edges);
    array_reset(world->zones);
}


static element_id_t edge_get_other_vertex(const edge_t *edge, element_id_t vertex_id) {
    assert(vertex_id != ID_NONE);
    if (vertex_id == edge->vertex_ids[0]) {
        return edge->vertex_ids[1];
    }
    else if (vertex_id == edge->vertex_ids[1]) {
        return edge->vertex_ids[0];
    }
    return ID_NONE;
}


static uint32_t vertex_get_connected_edge_index(const vertex_t *vertex, element_id_t edge_id) {
    for (uint32_t i = 0; i < vertex->edge_ids_num; ++i) {
        if (vertex->edge_ids[i] == edge_id) {
            return i;
        }
    }
    return INDEX_NONE;
}


static element_id_t vertex_get_prev_edge(const vertex_t *vertex, element_id_t edge_id) {
    uint32_t i = vertex_get_connected_edge_index(vertex, edge_id);
    assert(i != INDEX_NONE);
    return (i == 0) ? array_get_last(vertex->edge_ids) : vertex->edge_ids[i - 1];
}


static element_id_t vertex_get_next_edge(const vertex_t *vertex, element_id_t edge_id) {
    uint32_t i = vertex_get_connected_edge_index(vertex, edge_id);
    assert(i != INDEX_NONE);
    return (i + 1 == vertex->edge_ids_num) ? array_get_first(vertex->edge_ids) : vertex->edge_ids[i + 1];
}


static element_id_t contour_get_start_vertex(const contour_t *contour, const edge_t edges[]) {
    assert(contour->edge_ids_num > 2);
    const edge_t *start_edge = &edges[contour->edge_ids[0]];
    const edge_t *next_edge = &edges[contour->edge_ids[1]];

    if (start_edge->vertex_ids[0] == next_edge->vertex_ids[0] || start_edge->vertex_ids[0] == next_edge->vertex_ids[1]) {
        return start_edge->vertex_ids[1];
    }
    else if (start_edge->vertex_ids[1] == next_edge->vertex_ids[0] || start_edge->vertex_ids[1] == next_edge->vertex_ids[1]) {
        return start_edge->vertex_ids[0];
    }
    return ID_NONE;
}


const element_id_t *contour_get_vertices(const contour_t *contour, const edge_t edges[], arena_t *arena) {
    element_id_t *vertex_ids = arena_new_n(element_id_t, contour->edge_ids_num, arena);
    element_id_t vertex_id = contour_get_start_vertex(contour, edges);
    for (uint32_t i = 0; i < contour->edge_ids_num; ++i) {
        vertex_ids[i] = vertex_id;
        vertex_id = edge_get_other_vertex(&edges[contour->edge_ids[i]], vertex_id);
    }
    return vertex_ids;
}


static bool contour_is_valid(const contour_t *contour) {
    return !array_is_empty(contour->edge_ids);
}


static int32_t contour_get_winding(const contour_t *contour, const vertex_t vertices[], const edge_t edges[]) {
    element_id_t v0_id = contour_get_start_vertex(contour, edges);
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


static bool contour_is_perimeter(const contour_t *contour, const vertex_t vertices[], const edge_t edges[]) {
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) > 0;
}


static bool contour_is_hole(const contour_t *contour, const edge_t edges[], const vertex_t vertices[]) {
    return contour_is_valid(contour) && contour_get_winding(contour, vertices, edges) < 0;
}


contour_t contour_make(const vertex_t vertices[], const edge_t edges[], element_id_t edge_id, element_id_t start_vertex_id, arena_t *arena) {
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


static void world_replace_ids(element_id_t ids[], uint32_t ids_num, element_id_t from, element_id_t to) {
    for (uint32_t i = 0; i < ids_num; ++i) {
        if (ids[i] == from) {
            ids[i] = to;
        }
    }
}


element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *arena) {
    element_id_t vertex_id = (element_id_t)array_add(
        world->vertices,
        arena,
        (vertex_t){
            .position = position
        }
    );
    return vertex_id;
}


bool world_reindex_vertex(world_t *world, element_id_t old_index, element_id_t new_index) {
    if (!array_is_empty(world->vertices[new_index].edge_ids)) {
        // It's an error if we use a vertex which still has edges connected to it
        return false;
    }

    for (uint32_t i = 0; i < world->edges_num; ++i) {
        world_replace_ids(world->edges[i].vertex_ids, 2, old_index, new_index);
    }

    for (uint32_t i = 0; i < world->zones_num; ++i) {
        zone_t *zone = &world->zones[i];
        world_replace_ids(
            zone->perimeter.edge_ids,
            zone->perimeter.edge_ids_num,
            old_index,
            new_index
        );

        for (uint32_t j = 0; j < zone->holes_num; ++j) {
            world_replace_ids(
                zone->holes[j].edge_ids,
                zone->holes[j].edge_ids_num,
                old_index,
                new_index
            );
        }
    }

    world->vertices[new_index] = world->vertices[old_index];
    return true;
}


static void world_add_vertex_edge(world_t *world, element_id_t vertex_id, element_id_t edge_id, arena_t *arena) {
    vertex_t *vertices = world->vertices; 
    const edge_t *edges = world->edges;

    // Maintain a list of all the edges which connect to this vertex
    // Only create the array the first time we need it
    vertex_t *vertex = &vertices[vertex_id];
    if (!array_is_valid(vertex->edge_ids)) {
        array_reserve(vertex->edge_ids, arena, 16);
    }

    element_id_t v0 = edge_get_other_vertex(&edges[edge_id], vertex_id);
    assert(v0 != ID_NONE);
    vec2i_t p0 = vertices[v0].position;

    // Insert the edge such that the array of edges at the vertex is ordered in a counter clockwise fan.
    // When we arrive at this vertex from an edge, the next edge in the array is the "most clockwise" one,
    // which we can use to build a contour.
    uint32_t insert_index = 0;
    for (; insert_index < vertex->edge_ids_num; ++insert_index) {
        element_id_t v1 = edge_get_other_vertex(&edges[vertex->edge_ids[insert_index]], vertex_id);
        assert(v1 != ID_NONE);
        vec2i_t p1 = vertices[v1].position;

        if (vec2i_wedge(vec2i_sub(p0, vertex->position), vec2i_sub(p1, vertex->position)) < 0) {
            break;
        }
    }

    array_insert(vertex->edge_ids, arena, insert_index, edge_id);
}


static void world_find_zone_holes(world_t *world, element_id_t zone_id) {

}


static void zone_build_subzones(zone_t *zone, const vertex_t vertices[], const edge_t edges[], arena_t *arena, arena_t scratch) {
    const element_id_t *vertex_ids = contour_get_vertices(&zone->perimeter, edges, &scratch);
    uint32_t num_vertices = zone->perimeter.edge_ids_num;

    uint32_t i = array_push(zone->subzones, arena);

    array_init_reserve(zone->subzones[i].vertex_ids, arena, 32);
    array_resize(zone->subzones[i].vertex_ids, arena, num_vertices);

    for (uint32_t j = 0; j < num_vertices; ++j) {
        zone->subzones[i].vertex_ids[j] = vertex_ids[j];
    }
}


static element_id_t world_add_zone(world_t *world, const contour_t *contour, arena_t *arena, arena_t scratch) {
    element_id_t zone_id = (element_id_t)array_add(
        world->zones,
        arena,
        (zone_t) {
            .outer_zone_id = ID_NONE,
            .floor_height = 192,
            .ceiling_height = 128,
            .floor_colour = 1,
            .ceiling_colour = 1
        }
    );

    zone_t *zone = &world->zones[zone_id];
    const vertex_t *vertices = world->vertices;
    const edge_t *edges = world->edges;

    array_init_copy_reserve(zone->perimeter.edge_ids, arena, contour->edge_ids, 32);
    array_init_reserve(zone->holes, arena, 8);
    array_init_reserve(zone->subzones, arena, 32);
    array_init_reserve(zone->inner_zone_ids, arena, 32);

    zone->aabb = zone_get_aabb(zone, edges, vertices);

    world_find_zone_holes(world, zone_id);
    zone_build_subzones(zone, vertices, edges, arena, scratch);

    // @todo: find zone hierarchy

    return zone_id;
}


element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *arena, arena_t scratch) {
    element_id_t edge_id = (element_id_t)array_add(
        world->edges,
        arena,
        (edge_t) {
            .vertex_ids = {v0, v1},
            .zone_ids = {ID_NONE, ID_NONE},
            .lower_colour = lower_colour,
            .upper_colour = upper_colour
        }
    );

    world_add_vertex_edge(world, v0, edge_id, arena);
    world_add_vertex_edge(world, v1, edge_id, arena);

    if (world->vertices[v0].edge_ids_num > 1 && world->vertices[v1].edge_ids_num > 1) {
        contour_t c0 = contour_make(world->vertices, world->edges, edge_id, v0, &scratch);
        contour_t c1 = contour_make(world->vertices, world->edges, edge_id, v1, &scratch);

        bool c0_is_perimeter = contour_is_perimeter(&c0, world->vertices, world->edges);
        bool c1_is_perimeter = contour_is_perimeter(&c1, world->vertices, world->edges);

        if (c0_is_perimeter && c1_is_perimeter) {
            // split existing zone
        }
        else if (c0_is_perimeter) {
            // create a new zone from contour 0
            world_add_zone(world, &c0, arena, scratch);
        }
        else if (c1_is_perimeter) {
            // create a new zone from contour 1
            world_add_zone(world, &c1, arena, scratch);
        }
    }

    return edge_id;
}



aabb2f_t edge_get_aabb(const edge_t *edge, const vertex_t vertices[]) {
    vec2f_t world_start_pos = vec2f_make_from_vec2i(vertices[edge->vertex_ids[0]].position);
    vec2f_t world_end_pos   = vec2f_make_from_vec2i(vertices[edge->vertex_ids[1]].position);
    return aabb2f_make(world_start_pos, world_end_pos);
}


aabb2f_t zone_get_aabb(const zone_t *zone, const edge_t edges[], const vertex_t vertices[]) {
    const element_id_t *edge_ids = zone->perimeter.edge_ids;

    aabb2f_t aabb = edge_get_aabb(&edges[edge_ids[0]], vertices);
    for (uint32_t i = 1; i < zone->perimeter.edge_ids_num; ++i) {
        aabb = aabb2f_union(aabb, edge_get_aabb(&edges[edge_ids[i]], vertices));
    }
    return aabb;
}


element_id_t vertex_find_closest_to_point(const vertex_t vertices[], uint32_t num_vertices, vec2f_t point, float within) {
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < num_vertices; ++i) {
        float distance_sqr = vec2f_lengthsqr(vec2f_sub(point, vec2f_make_from_vec2i(vertices[i].position)));
        if (distance_sqr <= tolerance_sqr) {
            result = (element_id_t)i;
            tolerance_sqr = distance_sqr;
        }
    }
    return result;
}


static float edge_point_distancesqr(vec2f_t start, vec2f_t end, vec2f_t point) {
    vec2f_t delta = vec2f_sub(end, start);
    float lengthsqr = vec2f_lengthsqr(delta);
    if (lengthsqr == 0.0f) {
        // We don't expect zero length edges -
        // But if they occur, the distance is simply the distance from point to the coincident endpoints
        return vec2f_lengthsqr(vec2f_sub(start, point));
    }

    // Calculate the projection distance along the edge (0, lengthsqr)
    // Distances outside this range get interpreted as the distance from the closest endpoint
    float dot = vec2f_dot(vec2f_sub(point, start), delta);
    if (dot < 0.0f) {
        return vec2f_lengthsqr(vec2f_sub(start, point));
    } else if (dot > lengthsqr) {
        return vec2f_lengthsqr(vec2f_sub(end, point));
    }

    vec2f_t projection = vec2f_add(start, vec2f_scalar_mul(delta, dot / lengthsqr));
    return vec2f_lengthsqr(vec2f_sub(projection, point));
}


element_id_t edge_find_closest_to_point(const edge_t edges[], uint32_t num_edges, const vertex_t vertices[], vec2f_t point, float within) {
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < num_edges; ++i) {
        vec2f_t p0 = vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[0]].position);
        vec2f_t p1 = vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[1]].position);
        aabb2f_t edge_aabb = aabb2f_make_with_margin(p0, p1, within);
        if (aabb2f_contains_point(edge_aabb, point)) {
            float distance_sqr = edge_point_distancesqr(p0, p1, point);
            if (distance_sqr <= tolerance_sqr) {
                result = (element_id_t)i;
                tolerance_sqr = distance_sqr;
            }
        }
    }
    return result;
}
