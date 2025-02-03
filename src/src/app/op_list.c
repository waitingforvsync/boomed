#include "app/op_list.h"
#include "base/arena.h"


op_list_t op_list_make(arena_t *ops_list_arena, arena_t *ops_arena, boomed_t *boomed, uint32_t capacity) {
    return (op_list_t) {
        .ops_list_arena = ops_list_arena,
        .ops_arena = ops_arena,
        .boomed = boomed,
        .ops = ops_make(ops_list_arena, capacity),
        .cursor = 0
    };
}


void op_list_reset(op_list_t *op_list) {
    ops_reset(&op_list->ops);
}


bool op_list_exec(op_list_t *op_list) {
    while (op_list->cursor < op_list->ops.num) {
        op_t *op = ops_get_ptr(&op_list->ops, op_list->cursor);
        if (op->type == op_type_sentinel) {
            return true;
        }
        if (!op_exec(op, op_list->boomed, op_list->ops_arena)) {
            return false;
        }
        op_list->cursor++;
    }
    return true;
}


bool op_list_undo(op_list_t *op_list) {
    while (op_list->cursor > 0) {
        op_list->cursor--;
        op_t *op = ops_get_ptr(&op_list->ops, op_list->cursor);
        if (op->type == op_type_sentinel) {
            return true;
        }
        if (!op_undo(op, op_list->boomed, op_list->ops_arena)) {
            return false;
        }
    }
    return true;
}


bool op_list_add(op_list_t *op_list, op_t op) {
    ops_resize(&op_list->ops, op_list->ops_list_arena, op_list->cursor);
    ops_add(&op_list->ops, op_list->ops_list_arena, op);
    return op_list_exec(op_list);
}
