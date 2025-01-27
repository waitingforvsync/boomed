#include <stdbool.h>
#include "boomed/defines.h"
#include "boomed/ops.h"
#include "boomed/boomed.h"
#include "boomed/world/world.h"


static void op_vertex_add_exec(op_t *op, boomed_t *boomed) {
    world_add_vertex(&boomed->world, op->vertex_add.position, &boomed->ids_arena);
}

static void op_vertex_add_undo(op_t *op, boomed_t *boomed) {
    world_remove_last_vertex(&boomed->world);
}


static void op_vertex_move_exec(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}

static void op_vertex_move_undo(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


static void op_edge_add_exec(op_t *op, boomed_t *boomed) {
    world_add_edge(
        &boomed->world,
        op->edge_add.vertices[0],
        op->edge_add.vertices[1],
        op->edge_add.upper_colour,
        op->edge_add.lower_colour,
        &boomed->ids_arena,
        boomed->scratch_arena
    );
}

static void op_edge_add_undo(op_t *op, boomed_t *boomed) {
    world_remove_last_edge(&boomed->world);
}


struct {
    void (*exec_fn)(op_t *, boomed_t *);
    void (*undo_fn)(op_t *, boomed_t *);
}
op_fns[] = {
    [op_type_vertex_add]    = {op_vertex_add_exec,    op_vertex_add_undo},
    [op_type_vertex_move]   = {op_vertex_move_exec,   op_vertex_move_undo},
    [op_type_edge_add]      = {op_edge_add_exec,      op_edge_add_undo}
};


uint32_t exec_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index) {
    assert(ops[index].type == op_type_sentinel);
    if (index != ops_num - 1) {
        for (++index; ops[index].type != op_type_sentinel; ++index) {
            if (op_fns[ops[index].type].exec_fn) {
                op_fns[ops[index].type].exec_fn(ops + index, boomed);
            }
        }
    }
    return index;
}


uint32_t undo_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index) {
    assert(ops[index].type == op_type_sentinel);
    if (index != 0) {
        for (--index; ops[index].type != op_type_sentinel; --index) {
            if (op_fns[ops[index].type].undo_fn) {
                op_fns[ops[index].type].undo_fn(ops + index, boomed);
            }
        }
    }
    return index;
}
