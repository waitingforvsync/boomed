#include "app/boomed.h"


void boomed_init(boomed_t *boomed) {
    boomed->ids_arena = arena_make();
    boomed->preview_ids_arena = arena_make();
    boomed->scratch_arena = arena_make();
    boomed->ops_arena = arena_make();

    world_init(&boomed->world);

    boomed->ops = ops_make(&boomed->ops_arena, 16384);
}


void boomed_deinit(boomed_t *boomed) {
    ops_reset(&boomed->ops);
    arena_deinit(&boomed->ops_arena);
    arena_deinit(&boomed->scratch_arena);
    arena_deinit(&boomed->preview_ids_arena);
    arena_deinit(&boomed->ids_arena);
}
