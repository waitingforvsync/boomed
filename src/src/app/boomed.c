#include "app/boomed.h"


void boomed_init(boomed_t *boomed) {
    boomed->ids_arena = arena_make();
    boomed->preview_arena = arena_make();
    boomed->scratch_arena = arena_make();
    boomed->ops_arena = arena_make();
    world_init(&boomed->world);
    boomed->op_list = op_list_make(&boomed->ops_arena, &boomed->ids_arena, boomed, 16384);
}


void boomed_deinit(boomed_t *boomed) {
    op_list_reset(&boomed->op_list);
    op_list_reset(&boomed->preview_op_list);
    arena_deinit(&boomed->ops_arena);
    arena_deinit(&boomed->scratch_arena);
    arena_deinit(&boomed->preview_arena);
    arena_deinit(&boomed->ids_arena);
}
