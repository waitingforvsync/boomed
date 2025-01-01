#include "boomed/world.h"
#include "boomed/generic/is_less.h"
#include "boomed/generic/lower_bound.h"

DEF_IS_LESS(element_id, element_id_t)
DEF_LOWER_BOUND(element_id, element_id_t)


world_t make_world(void) {
    world_t world = {0};
    world_init(&world);
    return world;
}


void world_init(world_t *world) {
    // Create separate arenas for the big arrays
    // These will only hold the single array allocation and thus must be resizable
    world->vertex_arena = make_arena();
    world->edge_arena = make_arena();
    world->zone_arena = make_arena();

    // Create arenas for the multiple small arrays; these can be chained
    world->vertex_edges_arena = make_arena();
    world->zone_contours_arena = make_arena();

    world->vertices = make_array_vertex(&world->vertex_arena, 256);
    world->edges = make_array_edge(&world->edge_arena, 256);
    world->zones = make_array_zone(&world->zone_arena, 256);
}


void world_deinit(world_t *world) {    
    arena_deinit(&world->zone_contours_arena);
    arena_deinit(&world->vertex_edges_arena);
    arena_deinit(&world->zone_arena);
    arena_deinit(&world->edge_arena);
    arena_deinit(&world->vertex_arena);
}
