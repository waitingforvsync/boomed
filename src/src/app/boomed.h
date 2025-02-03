#ifndef BOOMED_H_
#define BOOMED_H_

#include "app/op_list.h"
#include "base/arena.h"
#include "world/world.h"

typedef struct boomed_t boomed_t;

struct boomed_t {
    arena_t ids_arena;
    arena_t preview_arena;
    arena_t scratch_arena;
    arena_t ops_arena;
    world_t world;
    op_list_t op_list;
    op_list_t preview_op_list;
};


void boomed_init(boomed_t *boomed);
void boomed_deinit(boomed_t *boomed);


#endif // ifndef BOOMED_H_
