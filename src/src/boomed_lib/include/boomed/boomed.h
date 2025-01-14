#ifndef BOOMED_BOOMED_H_
#define BOOMED_BOOMED_H_

#include "boomed/arena.h"
#include "boomed/world.h"
#include "boomed/ops.h"

typedef struct boomed_t boomed_t;

struct boomed_t {
    arena_t world_arena;
    arena_t preview_arena;
    arena_t scratch_arena;
    arena_t ops_arena;
    world_t world;
    array_t(op_t, ops);
};


void boomed_init(boomed_t *boomed);
void boomed_deinit(boomed_t *boomed);


#endif // ifndef BOOMED_BOOMED_H_
