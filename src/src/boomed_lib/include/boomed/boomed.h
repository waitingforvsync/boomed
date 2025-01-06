#ifndef BOOMED_BOOMED_H_
#define BOOMED_BOOMED_H_

#include "boomed/arena.h"
#include "boomed/world.h"
#include "boomed/ops.h"

typedef struct boomed_t boomed_t;

struct boomed_t {
    arena_t ops_arena;
    array_op_t ops;
    world_t world;
};


#endif // ifndef BOOMED_BOOMED_H_
