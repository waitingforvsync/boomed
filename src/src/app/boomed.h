#ifndef BOOMED_H_
#define BOOMED_H_

#include "app/ops.h"
#include "base/arena.h"
#include "world/world.h"

typedef struct boomed_t boomed_t;

struct boomed_t {
    arena_t ids_arena;
    arena_t preview_ids_arena;
    arena_t scratch_arena;
    arena_t ops_arena;
    world_t world;
    ops_t ops;
};


void boomed_init(boomed_t *boomed);
void boomed_deinit(boomed_t *boomed);


#endif // ifndef BOOMED_H_
