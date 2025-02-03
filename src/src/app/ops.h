#ifndef OPS_H_
#define OPS_H_

#include <stdbool.h>
#include <stdint.h>
#include "math/vec2i.h"

typedef struct boomed_t boomed_t;
typedef uint16_t element_id_t;

typedef struct op_t op_t;
typedef struct op_vertex_add_t op_vertex_add_t;
typedef struct op_vertex_move_t op_vertex_move_t;
typedef struct op_vertex_merge_t op_vertex_merge_t;
typedef struct op_vertex_delete_t op_vertex_delete_t;
typedef struct op_edge_add_t op_edge_add_t;
typedef struct op_edge_split_t op_edge_split_t;
typedef struct op_edge_delete_t op_edge_delete_t;

enum op_type_t {
    op_type_sentinel,
    op_type_vertex_add,
    op_type_vertex_move,
    op_type_vertex_merge,
    op_type_vertex_delete,
    op_type_edge_add,
    op_type_edge_split,
    op_type_edge_delete
};


struct op_vertex_add_t {
    vec2i_t position;
};


struct op_vertex_move_t {
    element_id_t vertex_id;
    vec2i_t new_position;
    vec2i_t old_position;
};


struct op_vertex_merge_t {
    element_id_t vertex_to_merge_id;
    element_id_t combined_vertex_id;
};


struct op_vertex_delete_t {
    element_id_t vertex_id;
};


struct op_edge_add_t {
    element_id_t vertices[2];
    uint8_t upper_colour;
    uint8_t lower_colour;
};


struct op_edge_split_t {
    element_id_t edge_id;
};


struct op_edge_delete_t {
    element_id_t edge_id;
};


struct op_t {
    uint32_t type;
    union {
        op_vertex_add_t vertex_add;
        op_vertex_move_t vertex_move;
        op_vertex_merge_t vertex_merge;
        op_vertex_delete_t vertex_delete;
        op_edge_add_t edge_add;
        op_edge_split_t edge_split;
        op_edge_delete_t edge_delete;
    };
};

// Define op_t array type
#define TEMPLATE_NAME ops
#define TEMPLATE_TYPE op_t
#include "templates/array.h.template"


bool op_exec(op_t *op, boomed_t *boomed, arena_t *arena);
bool op_undo(op_t *op, boomed_t *boomed, arena_t *arena);


#endif // ifndef OPS_H_
