#include "base/arena.h"
#include "world/world.h"
#include "world/edge.h"
#include "world/subzone.h"
#include "world/zone.h"


void world_init(world_t *world) {
    assert(world);
    world->arena = arena_make();
    world->vertices = vertices_make(&world->arena, 8192);
    world->edges = edges_make(&world->arena, 8192);
    world->zones = zones_make(&world->arena, 1024);
}


void world_reset(world_t *world) {
    assert(world);
    vertices_reset(&world->vertices);
    edges_reset(&world->edges);
    zones_reset(&world->zones);
    arena_reset(&world->arena);
}


element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *ids_arena) {
    assert(world);
    element_id_t vertex_id = (element_id_t)vertices_add(
        &world->vertices,
        &world->arena,
        &(vertex_t) {
            .position = position,
            .edge_ids = element_ids_make(ids_arena, 32)
        }
    );
    return vertex_id;
}


bool world_remove_last_vertex(world_t *world) {
    assert(world);
    assert(element_ids_is_empty(&vertices_get_last(&world->vertices)->edge_ids));
    vertices_pop(&world->vertices);
    return true;
}


static void world_replace_ids(element_ids_slice_t ids, element_id_t from, element_id_t to) {
    for (uint32_t i = 0; i < ids.num; ++i) {
        if (element_ids_slice_get(ids, i) == from) {
            element_ids_slice_set(ids, i, to);
        }
    }
}

bool world_reindex_vertex(world_t *world, element_id_t old_index, element_id_t new_index) {
    assert(world);
    if (!element_ids_is_empty(&vertices_get(&world->vertices, new_index)->edge_ids)) {
        // It's an error if we use a vertex which still has edges connected to it
        return false;
    }

    edges_slice_t edges = world->edges.slice;

    for (uint32_t i = 0; i < edges.num; ++i) {
        edge_t *edge = edges_slice_get(edges, i);
        element_ids_slice_t element_ids = element_ids_slice_make(edge->vertex_ids, 2);
        world_replace_ids(element_ids, old_index, new_index);
    }

    for (uint32_t i = 0; i < world->zones.num; ++i) {
        zone_t *zone = zones_get(&world->zones, i);
        world_replace_ids(
            zone->perimeter.edge_ids.slice,
            old_index,
            new_index
        );

        for (uint32_t j = 0; j < zone->holes.num; ++j) {
            world_replace_ids(
                contours_get(&zone->holes, j)->edge_ids.slice,
                old_index,
                new_index
            );
        }
    }

    vertices_set(&world->vertices, new_index, vertices_get(&world->vertices, old_index));
    return true;
}


static void world_add_vertex_edge(world_t *world, element_id_t vertex_id, element_id_t edge_id, arena_t *ids_arena) {
    assert(world);
    edges_view_t edges = world->edges.view;

    // Maintain a list of all the edges which connect to this vertex
    vertex_t *vertex = vertices_get(&world->vertices, vertex_id);

    // This edge must not already be in the connected edges list
    assert(vertex_get_connected_edge_index(vertex, edge_id) == INDEX_NONE);

    element_id_t v0 = edge_get_other_vertex(edges_view_get(edges, edge_id), vertex_id);
    assert(v0 != ID_NONE);
    vec2i_t p0 = vertices_get(&world->vertices, v0)->position;

    // Insert the edge such that the array of edges at the vertex is ordered in a counter clockwise fan.
    // When we arrive at this vertex from an edge, the next edge in the array is the "most clockwise" one,
    // which we can use to build a contour.
    element_ids_t *edge_ids = &vertex->edge_ids;
    uint32_t insert_index = 0;
    for (; insert_index < vertex->edge_ids.num; ++insert_index) {
        const edge_t *compare_edge = edges_view_get(edges, element_ids_get(edge_ids, insert_index));
        element_id_t v1 = edge_get_other_vertex(compare_edge, vertex_id);
        assert(v1 != ID_NONE);
        vec2i_t p1 = vertices_get(&world->vertices, v1)->position;

        if (vec2i_wedge(vec2i_sub(p0, vertex->position), vec2i_sub(p1, vertex->position)) < 0) {
            break;
        }
    }

    element_ids_insert(edge_ids, ids_arena, insert_index, 1);
    element_ids_set(edge_ids, insert_index, edge_id);
}


element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *ids_arena, arena_t scratch) {
    assert(world);
    assert(v0 != ID_NONE);
    assert(v1 != ID_NONE);
    assert(v0 != v1);

    element_id_t edge_id = (element_id_t)edges_add(
        &world->edges,
        &world->arena,
        &(edge_t) {
            .vertex_ids = {v0, v1},
            .zone_ids = {ID_NONE, ID_NONE},
            .lower_colour = lower_colour,
            .upper_colour = upper_colour
        }
    );

    world_add_vertex_edge(world, v0, edge_id, ids_arena);
    world_add_vertex_edge(world, v1, edge_id, ids_arena);

    if (vertices_get(&world->vertices, v0)->edge_ids.num > 1 &&
        vertices_get(&world->vertices, v1)->edge_ids.num > 1) {
        contour_t c0 = contour_make(world->vertices.view, world->edges.view, edge_id, v0, &scratch);
        contour_t c1 = contour_make(world->vertices.view, world->edges.view, edge_id, v1, &scratch);

        bool c0_is_perimeter = contour_is_perimeter(&c0, world->vertices.view, world->edges.view);
        bool c1_is_perimeter = contour_is_perimeter(&c1, world->vertices.view, world->edges.view);

        if (c0_is_perimeter && c1_is_perimeter) {
            // split existing zone
        }
        else if (c0_is_perimeter) {
            // create a new zone from contour 0
            world_add_zone(world, &c0, ids_arena, scratch);
        }
        else if (c1_is_perimeter) {
            // create a new zone from contour 1
            world_add_zone(world, &c1, ids_arena, scratch);
        }
    }

    return edge_id;
}


bool world_remove_last_edge(world_t *world) {
    // @todo
    return true;
}


static void world_find_zone_holes(world_t *world, element_id_t zone_id) {
    assert(world);
}


element_id_t world_add_zone(world_t *world, const contour_t *contour, arena_t *ids_arena, arena_t scratch) {
    assert(world);
    assert(contour);
    element_id_t zone_id = (element_id_t)zones_add(
        &world->zones,
        &world->arena,
        &(zone_t) {
            .perimeter = contour_make_copy(contour, ids_arena),
            .holes = contours_make(ids_arena, 8),
            .subzones = subzones_make(ids_arena, 32),
            .inner_zone_ids = element_ids_make(ids_arena, 32),
            .outer_zone_id = ID_NONE,
            .floor_height = 192,
            .ceiling_height = 128,
            .floor_colour = 1,
            .ceiling_colour = 1
        }
    );

    zone_t *zone = zones_get(&world->zones, zone_id);
    vertices_view_t vertices = world->vertices.view;
    edges_view_t edges = world->edges.view;

    zone->aabb = zone_get_aabb(zone, vertices, edges);

    world_find_zone_holes(world, zone_id);
    zone_build_subzones(zone, vertices, edges, ids_arena, scratch);

    // @todo: find zone hierarchy

    return zone_id;
}


element_id_t world_find_vertex_closest_to_point(const world_t *world, vec2f_t point, float within) {
    assert(world);
    vertices_view_t vertices = world->vertices.view;
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < vertices.num; ++i) {
        vec2f_t position = vec2f_make_from_vec2i(vertices_view_get(vertices, i)->position);
        float distance_sqr = vec2f_lengthsqr(vec2f_sub(point, position));
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


element_id_t world_find_edge_closest_to_point(const world_t *world, vec2f_t point, float within) {
    assert(world);
    vertices_view_t vertices = world->vertices.view;
    edges_view_t edges = world->edges.view;
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < edges.num; ++i) {
        const edge_t *edge = edges_view_get(edges, i);
        vec2f_t p0 = vec2f_make_from_vec2i(vertices_view_get(vertices, edge->vertex_ids[0])->position);
        vec2f_t p1 = vec2f_make_from_vec2i(vertices_view_get(vertices, edge->vertex_ids[1])->position);
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
