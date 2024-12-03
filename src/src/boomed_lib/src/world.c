#include "boomed/world.h"


DEF_ARRAY_IMPL(element_id, element_id_t)
DEF_ARRAY_IMPL(vertex, vertex_t)
DEF_ARRAY_IMPL(edge, edge_t)
DEF_ARRAY_IMPL(contour, contour_t)
DEF_ARRAY_IMPL(zone, zone_t)


world_t make_world(void) {
    world_t world = {0};
    world_init(&world);
    return world;
}


void world_init(world_t *world) {
    // Create separate arenas for the big arrays
    // These will only hold the single array allocation and thus must be resizable
    world->vertex_arena = make_arena_resizable(0x10000);
    world->edge_arena = make_arena_resizable(0x10000);
    world->zone_arena = make_arena_resizable(0x10000);

    // Create arenas for the multiple small arrays; these can be chained
    world->vertex_edges_arena = make_arena_chained(0x10000);
    world->zone_contours_arena = make_arena_chained(0x10000);

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
