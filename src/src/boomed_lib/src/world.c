#include "boomed/world.h"
#include "boomed/math/aabb2f.h"
#include "boomed/math/vec2f.h"


void world_init(world_t *world) {
    // Create separate arenas for the big arrays
    // These will only hold the single array allocation and thus must be resizable
    world->vertex_arena = make_arena();
    world->edge_arena = make_arena();
    world->zone_arena = make_arena();

    // Create arenas for the multiple small arrays; these can be chained
    world->id_arena = make_arena();
    world->temp_id_arena = make_arena();

    array_init(world->vertices, 4096, &world->vertex_arena);
    array_init(world->edges, 4096, &world->edge_arena);
    array_init(world->zones, 4096, &world->zone_arena);

    //--- test code
    array_add(world->vertices, (vertex_t){.position = {0, 0}});
    array_add(world->vertices, (vertex_t){.position = {100, 25}});
    array_add(world->vertices, (vertex_t){.position = {0, 25}});

    array_add(world->edges, (edge_t){.vertex_ids = {0, 1}});
    array_add(world->edges, (edge_t){.vertex_ids = {0, 2}});
    //--- end test code
}


void world_deinit(world_t *world) {    
    arena_deinit(&world->temp_id_arena);
    arena_deinit(&world->id_arena);
    arena_deinit(&world->zone_arena);
    arena_deinit(&world->edge_arena);
    arena_deinit(&world->vertex_arena);
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


static element_id_t world_get_contour_start_vertex(const world_t *world, const contour_t *contour) {
    assert(contour->edge_ids_num > 2);
    const edge_t *start_edge = world->edges + contour->edge_ids[0];
    const edge_t *next_edge = world->edges + contour->edge_ids[1];

    if (start_edge->vertex_ids[0] == next_edge->vertex_ids[0] || start_edge->vertex_ids[0] == next_edge->vertex_ids[1]) {
        return start_edge->vertex_ids[1];
    }
    else if (start_edge->vertex_ids[1] == next_edge->vertex_ids[0] || start_edge->vertex_ids[1] == next_edge->vertex_ids[1]) {
        return start_edge->vertex_ids[0];
    }
    return ID_NONE;
}


static const element_id_t *world_get_contour_vertices(const world_t *world, const contour_t *contour, arena_t *arena) {
    element_id_t *vertex_ids = arena_new_n(element_id_t, contour->edge_ids_num, arena);
    element_id_t vertex_id = world_get_contour_start_vertex(world, contour);
    for (uint32_t i = 0; i < contour->edge_ids_num; ++i) {
        vertex_ids[i] = vertex_id;
        vertex_id = edge_get_other_vertex(world->edges + contour->edge_ids[i], vertex_id);
    }
    return vertex_ids;
}


static void world_replace_ids(element_id_t *ids, uint32_t ids_num, element_id_t from, element_id_t to) {
    for (uint32_t i = 0; i < ids_num; ++i) {
        if (ids[i] == from) {
            ids[i] = to;
        }
    }
}


element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *vertex_arena) {
    element_id_t vertex_id = (element_id_t)array_add(
        world->vertices,
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
        zone_t *zone = world->zones + i;
        world_replace_ids(zone->perimeter.edge_ids, zone->perimeter.edge_ids_num, old_index, new_index);
        for (uint32_t j = 0; j < zone->holes_num; ++j) {
            world_replace_ids(zone->holes[j].edge_ids, zone->holes[j].edge_ids_num, old_index, new_index);
        }
    }

    world->vertices[new_index] = world->vertices[old_index];
    return true;
}


static void world_add_vertex_edge(world_t *world, element_id_t vertex_id, element_id_t edge_id, arena_t *id_arena) {
    // Insert the edge such that the array of edges is ordered clockwise
    vertex_t *vertex = world->vertices + vertex_id;
    if (!array_is_valid(vertex->edge_ids)) {
        array_init(vertex->edge_ids, 16, id_arena);
    }
    // TODO
}

element_id_t world_add_edge(world_t *world, element_id_t v0, element_id_t v1, uint8_t upper_colour, uint8_t lower_colour, arena_t *edge_arena, arena_t *id_arena) {
    element_id_t edge_id = (element_id_t)array_add(
        world->edges,
        (edge_t){
            .vertex_ids = {v0, v1},
            .zone_ids = {ID_NONE, ID_NONE},
            .lower_colour = lower_colour,
            .upper_colour = upper_colour
        }
    );
    world_add_vertex_edge(world, v0, edge_id, id_arena);
    world_add_vertex_edge(world, v1, edge_id, id_arena);
    return edge_id;
}



aabb2f_t world_get_edge_aabb(const world_t *world, element_id_t edge_id) {
    const vertex_t *vertices = world->vertices;
    const edge_t *edges = world->edges;

    vec2f_t world_start_pos = vec2f_make_from_vec2i(vertices[edges[edge_id].vertex_ids[0]].position);
    vec2f_t world_end_pos   = vec2f_make_from_vec2i(vertices[edges[edge_id].vertex_ids[1]].position);
    return aabb2f_make(world_start_pos, world_end_pos);
}


aabb2f_t world_get_zone_aabb(const world_t *world, element_id_t zone_id) {
    const vertex_t *vertices = world->vertices;
    const zone_t *zones = world->zones;
    const element_id_t *edge_ids = zones[zone_id].perimeter.edge_ids;

    aabb2f_t aabb = world_get_edge_aabb(world, edge_ids[0]);
    for (uint32_t i = 1; i < zones[zone_id].perimeter.edge_ids_num; ++i) {
        aabb = aabb2f_union(aabb, world_get_edge_aabb(world, edge_ids[i]));
    }
    return aabb;
}


element_id_t world_find_vertex_at_position(const world_t *world, vec2f_t point, float within) {
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
        // But if they occur, the distance is simply the distance from point to either edge point
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


element_id_t world_find_edge_at_position(const world_t *world, vec2f_t point, float within) {
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
