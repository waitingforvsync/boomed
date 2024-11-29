#include "boomed/world.h"


world_t make_world(void) {
    world_t world = {0};
    world_init(&world);
    return world;
}


void world_init(world_t *world) {
    world->vertex_arena = make_arena(0x10000);
    world->edge_arena = make_arena(0x10000);
    world->zone_arena = make_arena(0x10000);
    world->vertex_edges_arena = make_arena(0x10000);
    world->zone_contours_arena = make_arena(0x10000);
}


void world_deinit(world_t *world) {
    arena_deinit(&world->zone_contours_arena);
    arena_deinit(&world->vertex_edges_arena);
    arena_deinit(&world->zone_arena);
    arena_deinit(&world->edge_arena);
    arena_deinit(&world->vertex_arena);
}
