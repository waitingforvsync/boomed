#include "app/boomed.h"
#include "app/ops.h"
#include "base/defines.h"
#include "world/world.h"


static bool op_vertex_add_exec(op_t *op, boomed_t *boomed, arena_t *arena) {
    element_id_t vertex_id = world_add_vertex(&boomed->world, op->vertex_add.position, arena);
    return (vertex_id != ID_NONE);
}

static bool op_vertex_add_undo(op_t *op, boomed_t *boomed, arena_t *arena) {
    return world_remove_last_vertex(&boomed->world);;
}


static bool op_vertex_move_exec(op_t *op, boomed_t *boomed, arena_t *arena) {
    return true;
}

static bool op_vertex_move_undo(op_t *op, boomed_t *boomed, arena_t *arena) {
    return true;
}


static bool op_edge_add_exec(op_t *op, boomed_t *boomed, arena_t *arena) {
    element_id_t edge_id = world_add_edge(
        &boomed->world,
        op->edge_add.vertices[0],
        op->edge_add.vertices[1],
        op->edge_add.upper_colour,
        op->edge_add.lower_colour,
        arena,
        boomed->scratch_arena
    );
    return (edge_id != ID_NONE);
}

static bool op_edge_add_undo(op_t *op, boomed_t *boomed, arena_t *arena) {
    return world_remove_last_edge(&boomed->world);
}


static struct {
    bool (*exec_fn)(op_t *, boomed_t *, arena_t *);
    bool (*undo_fn)(op_t *, boomed_t *, arena_t *);
}
op_fns[] = {
    [op_type_vertex_add]    = {op_vertex_add_exec,    op_vertex_add_undo},
    [op_type_vertex_move]   = {op_vertex_move_exec,   op_vertex_move_undo},
    [op_type_edge_add]      = {op_edge_add_exec,      op_edge_add_undo}
};


bool op_exec(op_t *op, boomed_t *boomed, arena_t *arena) {
    if (op_fns[op->type].exec_fn) {
        return op_fns[op->type].exec_fn(op, boomed, arena);
    }
    return true;
}


bool op_undo(op_t *op, boomed_t *boomed, arena_t *arena) {
    if (op_fns[op->type].undo_fn) {
        return op_fns[op->type].undo_fn(op, boomed, arena);
    }
    return true;
}

