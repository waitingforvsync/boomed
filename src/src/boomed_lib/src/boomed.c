#include "boomed/boomed.h"


void boomed_init(boomed_t *boomed) {
    boomed->world_arena = make_arena();
    boomed->preview_arena = make_arena();
    boomed->scratch_arena = make_arena();
    boomed->ops_arena = make_arena();

    world_init(&boomed->world, &boomed->world_arena);

    array_init_reserve(boomed->ops, &boomed->ops_arena, 16384);
}


void boomed_deinit(boomed_t *boomed) {
    array_reset(boomed->ops);
    arena_deinit(&boomed->ops_arena);
    arena_deinit(&boomed->scratch_arena);
    arena_deinit(&boomed->preview_arena);
    arena_deinit(&boomed->world_arena);
}
