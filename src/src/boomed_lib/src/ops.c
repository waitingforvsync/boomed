#include <assert.h>
#include "boomed/ops.h"
#include "boomed/boomed.h"


static void null_op(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


static void do_add_vertex(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


static void undo_add_vertex(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


static void do_add_edge(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


static void undo_add_edge(op_t *op, boomed_t *boomed) {
    (void)op;
    (void)boomed;
}


typedef struct op_fn_t {
    void (*do_fn)(op_t *, boomed_t *);
    void (*undo_fn)(op_t *, boomed_t *);
} op_fn_t;


op_fn_t op_fns[] = {
    [op_type_sentinel]   = {null_op, null_op},
    [op_type_add_vertex] = {do_add_vertex, undo_add_vertex},
    [op_type_add_edge]   = {do_add_edge, undo_add_edge}
};


uint32_t do_compound_op(boomed_t *boomed, slice_op_t ops, uint32_t index) {
    assert(ops.data[index].type == op_type_sentinel);
    if (index != ops.size - 1) {
        for (++index; ops.data[index].type != op_type_sentinel; ++index) {
            op_fns[ops.data[index].type].do_fn(ops.data + index, boomed);
        }
    }
    return index;
}


uint32_t undo_compound_op(boomed_t *boomed, slice_op_t ops, uint32_t index) {
    assert(ops.data[index].type == op_type_sentinel);
    if (index != 0) {
        for (--index; ops.data[index].type != op_type_sentinel; --index) {
            op_fns[ops.data[index].type].undo_fn(ops.data + index, boomed);
        }
    }
    return index;
}
