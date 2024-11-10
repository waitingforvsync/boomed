#ifndef BOOMED_WORLD_H_
#define BOOMED_WORLD_H_

#include "boomed/math/vec2i.h"
#include "boomed/generic/slice.h"


typedef struct vertex_t vertex_t;
typedef struct edge_t edge_t;
typedef struct contour_t contour_t;
typedef struct zone_t zone_t;

DEF_SLICE(vertex, vertex_t)
DEF_SLICE(edge, edge_t)
DEF_SLICE(contour, contour_t)

typedef struct world_t world_t;

typedef uint16_t element_index_t;

DEF_SLICE(element_index, element_index_t)


struct vertex_t {
    vec2i_t position;
    slice_edge_t edges;
};


struct edge_t {
    element_index_t vertices[2];
    element_index_t zones[2];
};


struct contour_t {
    slice_element_index_t edges;
};


struct zone_t {
    contour_t perimeter;
    slice_contour_t holes;
};


struct world_t {

};


element_index_t world_add_vertex(element_index_t index, vec2i_t position);
void world_reindex_vertex(element_index_t old_index, element_index_t new_index);
void world_remove_vertex(element_index_t index);



#endif // ifndef BOOMED_WORLD_H_
