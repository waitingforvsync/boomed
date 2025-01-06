#include "boomed/world.h"
#include "boomed/math/vec2f.h"
#include "boomed/generic/find.h"
#include "boomed/generic/max.h"
#include "boomed/generic/min.h"
#include "boomed/generic/min_element.h"
#include "boomed/generic/optional.h"


DEF_FIND_BY_PRED_CTX(vertex, vertex_t)
DEF_IS_LESS_OPTIONAL(float, float)
DEF_MIN_ELEMENT_BY_SCORE_CTX(edge, optional_float, edge_t, optional_float_t)


void world_init(world_t *world) {
    // Create separate arenas for the big arrays
    // These will only hold the single array allocation and thus must be resizable
    world->vertex_arena = make_arena();
    world->edge_arena = make_arena();
    world->zone_arena = make_arena();

    // Create arenas for the multiple small arrays; these can be chained
    world->id_arena = make_arena();
    world->temp_id_arena = make_arena();

    world->vertices = make_array_vertex(&world->vertex_arena, 256);
    world->edges = make_array_edge(&world->edge_arena, 256);
    world->zones = make_array_zone(&world->zone_arena, 256);

    //--- test code
    array_vertex_resize(&world->vertices, &world->vertex_arena, 3);
    world->vertices.data[0] = (vertex_t){.position = {0, 0}};
    world->vertices.data[1] = (vertex_t){.position = {100, 0}};
    world->vertices.data[2] = (vertex_t){.position = {0, 25}};

    array_edge_resize(&world->edges, &world->edge_arena, 2);
    world->edges.data[0] = (edge_t){.vertex_ids = {0, 1}};
    world->edges.data[1] = (edge_t){.vertex_ids = {0, 2}};
    //--- end test code
}


void world_deinit(world_t *world) {    
    arena_deinit(&world->temp_id_arena);
    arena_deinit(&world->id_arena);
    arena_deinit(&world->zone_arena);
    arena_deinit(&world->edge_arena);
    arena_deinit(&world->vertex_arena);
}



typedef struct {
    vec2f_t point;
    float half_snap;
} world_find_vertex_ctx_t;

static bool is_vertex_within_snap_distance(void *ctx, const vertex_t *vertex) {
    const world_find_vertex_ctx_t* find_vertex_ctx = ctx;
    return fabsf(find_vertex_ctx->point.x - vertex->position.x) <= find_vertex_ctx->half_snap &&
           fabsf(find_vertex_ctx->point.y - vertex->position.y) <= find_vertex_ctx->half_snap;
}


uint32_t world_find_vertex_at_position(const world_t *world, vec2f_t point, uint32_t snap) {
    return find_vertex_by_pred_ctx(
        world->vertices.const_slice,
        is_vertex_within_snap_distance,
        &(world_find_vertex_ctx_t) {point, snap * 0.5f}
    );
}


static optional_float_t distance_edge_point(vec2f_t start, vec2f_t end, vec2f_t point) {
    // First reject by bounding box
    if (point.x < min_float(start.x, end.x) ||
        point.x > max_float(start.x, end.x) ||
        point.y < min_float(start.y, end.y) ||
        point.y > max_float(start.y, end.y)
    ) {
        return (optional_float_t) {false};
    }

    // Next reject by zero-length edges
    vec2f_t delta = vec2f_sub(end, start);
    float lengthsqr = vec2f_lengthsqr(delta);

    if (lengthsqr == 0.0f) {
        return (optional_float_t) {false};
    }

    // Next reject if the perpendicular point is not on the segment
    float dot = vec2f_dot(vec2f_sub(point, start), delta);
    if (dot < 0.0f || dot > lengthsqr) {
        return (optional_float_t) {false};
    }

    // The point is perpendicular somewhere along the length of the segment,
    // so calculate its absolute distance
    vec2f_t projection = vec2f_add(start, vec2f_scalar_mul(delta, dot / lengthsqr));

    return (optional_float_t) {true, vec2f_length(vec2f_sub(projection, point))};
}

typedef struct {
    const world_t *world;
    vec2f_t point;
    float half_snap;
} world_find_edge_ctx_t;

static optional_float_t is_edge_within_snap_distance(void *ctx, const edge_t *edge) {
    const world_find_edge_ctx_t *find_edge_ctx = ctx;
    const vertex_t *vertices = find_edge_ctx->world->vertices.data;

    // Get perpendicular distance between this edge and the point
    // If the perpendicular projection doesn't lie on the edge segment, the return value is invalid
    optional_float_t distance = distance_edge_point(
        vec2f_make_from_vec2i(vertices[edge->vertex_ids[0]].position),
        vec2f_make_from_vec2i(vertices[edge->vertex_ids[1]].position),
        find_edge_ctx->point
    );

    // If the return value is valid, but is too far away to be considered, invalidate it
    if (distance.is_valid && distance.value > find_edge_ctx->half_snap) {
        distance.is_valid = false;
    }

    return distance;
}

uint32_t world_find_edge_at_position(const world_t *world, vec2f_t point, uint32_t snap) {
    return min_element_edge_by_optional_float_ctx(
        world->edges.const_slice,
        is_edge_within_snap_distance,
        (optional_float_t) {false},
        &(world_find_edge_ctx_t) {world, point, snap * 0.5f}
    );
}
