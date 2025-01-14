#ifndef BOOMED_OPS_H_
#define BOOMED_OPS_H_

#include <stdint.h>
#include "boomed/math/vec2i.h"

typedef struct boomed_t boomed_t;
typedef uint16_t element_id_t;


typedef struct op_t op_t;
typedef struct op_vertex_add_t op_vertex_add_t;
typedef struct op_vertex_connect_edge_t op_vertex_connect_edge_t;
typedef struct op_vertex_move_t op_vertex_move_t;
typedef struct op_edge_add_t op_edge_add_t;

enum op_type {
    op_type_sentinel,
    op_type_vertex_add,
    op_type_vertex_connect_edge,
    op_type_vertex_move,
    op_type_edge_add
};


struct op_vertex_add_t {
    vec2i_t position;
};


struct op_vertex_connect_edge_t {
    element_id_t vertex_id;
    element_id_t edge_id;
};


struct op_vertex_move_t {
    element_id_t vertex_id;
    vec2i_t new_position;
    vec2i_t old_position;
};


struct op_edge_add_t {
    element_id_t vertices[2];
    uint8_t upper_colour;
    uint8_t lower_colour;
};


struct op_t {
    uint32_t type;
    union {
        op_vertex_add_t vertex_add;
        op_vertex_move_t vertex_move;
        op_edge_add_t edge_add;
    };
};



uint32_t do_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index);
uint32_t undo_compound_op(boomed_t *boomed, op_t *ops, uint32_t ops_num, uint32_t index);

#endif // ifndef BOOMED_OPS_H_
