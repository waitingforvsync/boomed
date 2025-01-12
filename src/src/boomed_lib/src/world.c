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
    array_add(world->vertices, (vertex_t){.position = {100, 0}});
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


element_id_t world_add_vertex(world_t *world, vec2i_t position, arena_t *vertex_arena) {
    element_id_t vertex_id = (element_id_t)array_add(
        world->vertices,
        (vertex_t){
            .position = position
        }
    );
    return vertex_id;
}



uint32_t world_find_vertex_at_position(const world_t *world, vec2f_t point, float within) {
    const vertex_t *vertices = world->vertices;
    float tolerance_sqr = within * within;
    uint32_t result = INDEX_NONE;

    for (uint32_t i = 0; i < world->vertices_num; ++i) {
        float distance_sqr = vec2f_lengthsqr(vec2f_sub(point, vec2f_make_from_vec2i(vertices[i].position)));
        if (distance_sqr <= tolerance_sqr) {
            result = i;
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


uint32_t world_find_edge_at_position(const world_t *world, vec2f_t point, float within) {
    const vertex_t *vertices = world->vertices;
    const edge_t *edges = world->edges;
    float tolerance_sqr = within * within;
    uint32_t result = INDEX_NONE;

    for (uint32_t i = 0; i < world->edges_num; ++i) {
        vec2f_t p0 = vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[0]].position);
        vec2f_t p1 = vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[1]].position);
        aabb2f_t edge_aabb = aabb2f_make_with_margin(p0, p1, within);
        if (aabb2f_contains_point(edge_aabb, point)) {
            float distance_sqr = edge_point_distancesqr(p0, p1, point);
            if (distance_sqr <= tolerance_sqr) {
                result = i;
                tolerance_sqr = distance_sqr;
            }
        }
    }
    return result;
}
