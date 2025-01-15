#include "boomed/world/world.h"
#include "boomed/world/vertex.h"
#include "boomed/world/edge.h"
#include "boomed/world/subzone.h"
#include "boomed/world/zone.h"
#include "boomed/arena.h"


void world_init(world_t *world, arena_t *arena) {
    assert(world);
    assert(arena);
    array_init_reserve(world->vertices, arena, 8192);
    array_init_reserve(world->edges, arena, 8192);
    array_init_reserve(world->zones, arena, 1024);
}


void world_reset(world_t *world) {
    assert(world);
    array_reset(world->vertices);
    array_reset(world->edges);
    array_reset(world->zones);
}


element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *arena) {
    assert(world);
    element_id_t vertex_id = (element_id_t)array_add(
        world->vertices,
        arena,
        (vertex_t){
            .position = position
        }
    );
    return vertex_id;
}


bool world_remove_last_vertex(world_t *world) {
    assert(world);
    assert(array_is_empty(array_get_last(world->vertices).edge_ids));
    array_pop(world->vertices);
    return true;
}


static void world_replace_ids(element_id_t *ids, uint32_t ids_num, element_id_t from, element_id_t to) {
    assert(ids);
    for (uint32_t i = 0; i < ids_num; ++i) {
        if (ids[i] == from) {
            ids[i] = to;
        }
    }
}

bool world_reindex_vertex(world_t *world, element_id_t old_index, element_id_t new_index) {
    assert(world);
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
    assert(world);
    vertex_t *vertices = world->vertices; 
    const edge_t *edges = world->edges;

    // Maintain a list of all the edges which connect to this vertex
    // Only create the array the first time we need it
    vertex_t *vertex = &vertices[vertex_id];
    if (!array_is_valid(vertex->edge_ids)) {
        array_reserve(vertex->edge_ids, arena, 16);
    }

    // This edge must not already be in the connected edges list
    assert(vertex_get_connected_edge_index(vertex, edge_id) == INDEX_NONE);

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


element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *arena, arena_t scratch) {
    assert(world);
    assert(v0 != ID_NONE);
    assert(v1 != ID_NONE);
    assert(v0 != v1);

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


static void world_find_zone_holes(world_t *world, element_id_t zone_id) {
    assert(world);
}


element_id_t world_add_zone(world_t *world, const contour_t *contour, arena_t *arena, arena_t scratch) {
    assert(world);
    assert(contour);
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

    zone->aabb = zone_get_aabb(zone, vertices, edges);

    world_find_zone_holes(world, zone_id);
    zone_build_subzones(zone, vertices, edges, arena, scratch);

    // @todo: find zone hierarchy

    return zone_id;
}


element_id_t world_find_vertex_closest_to_point(const world_t *world, vec2f_t point, float within) {
    assert(world);
    const vertex_t *vertices = world->vertices;
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < world->vertices_num; ++i) {
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


element_id_t world_find_edge_closest_to_point(const world_t *world, vec2f_t point, float within) {
    assert(world);
    const vertex_t *vertices = world->vertices;
    const edge_t *edges = world->edges;
    float tolerance_sqr = within * within;
    element_id_t result = ID_NONE;

    for (uint32_t i = 0; i < world->edges_num; ++i) {
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
