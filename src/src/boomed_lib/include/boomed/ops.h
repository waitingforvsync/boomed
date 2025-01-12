#ifndef BOOMED_OPS_H_
#define BOOMED_OPS_H_

#include "boomed/world.h"

typedef struct boomed_t boomed_t;

typedef struct op_t op_t;
typedef struct op_sentinel_t op_sentinel_t;
typedef struct op_add_vertex_t op_add_vertex_t;
typedef struct op_add_edge_t op_add_edge_t;

enum op_type {
    op_type_sentinel,
    op_type_add_vertex,
    op_type_add_edge
};


struct op_add_vertex_t {
    vec2i_t position;
};


struct op_add_edge_t {
    element_id_t vertices[2];
};


struct op_t {
    uint32_t type;
    union {
        op_add_vertex_t add_vertex;
        op_add_edge_t add_edge;
    };
};



uint32_t do_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index);
uint32_t undo_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index);

#endif // ifndef BOOMED_OPS_H_
