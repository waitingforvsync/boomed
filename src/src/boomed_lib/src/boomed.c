#include "boomed/boomed.h"


void boomed_init(boomed_t *boomed) {
    boomed->ids_arena = make_arena();
    boomed->preview_ids_arena = make_arena();
    boomed->scratch_arena = make_arena();
    boomed->ops_arena = make_arena();

    world_init(&boomed->world);

    array_init_reserve(boomed->ops, &boomed->ops_arena, 16384);
}


void boomed_deinit(boomed_t *boomed) {
    array_reset(boomed->ops);
    arena_deinit(&boomed->ops_arena);
    arena_deinit(&boomed->scratch_arena);
    arena_deinit(&boomed->preview_ids_arena);
    arena_deinit(&boomed->ids_arena);
}
