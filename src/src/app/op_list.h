#ifndef OP_LIST_H_
#define OP_LIST_H_

#include "app/ops.h"

typedef struct op_list_t op_list_t;
typedef struct arena_t arena_t;
typedef struct boomed_t boomed_t;


struct op_list_t {
    arena_t *ops_list_arena;
    arena_t *ops_arena;
    boomed_t *boomed;
    ops_t ops;
    uint32_t cursor;
};


op_list_t op_list_make(arena_t *ops_list_arena, arena_t *ops_arena, boomed_t *boomed, uint32_t capacity);
void op_list_reset(op_list_t *op_list);
bool op_list_exec(op_list_t *op_list);
bool op_list_undo(op_list_t *op_list);
bool op_list_add(op_list_t *op_list, const op_t *op);


#endif // ifndef OP_LIST_H_
