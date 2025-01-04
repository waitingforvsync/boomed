#include "boomed/world.h"
#include "boomed/generic/is_less.h"
#include "boomed/generic/equal_range.h"

DEF_IS_LESS(element_id, element_id_t)
DEF_LOWER_BOUND(element_id, element_id_t)
DEF_UPPER_BOUND(element_id, element_id_t)
DEF_EQUAL_RANGE(element_id, element_id_t)


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
