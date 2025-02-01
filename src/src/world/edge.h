#ifndef EDGE_H_
#define EDGE_H_

#include "world/element_id.h"

typedef struct vertices_view_t vertices_view_t;
typedef struct edge_t edge_t;
typedef struct aabb2f_t aabb2f_t;


// Representation of an edge in the world.
// Edges are defined by the two vertices they connect (this is an unordered set),
// and have an upper and lower colour for when walls need to be rendered between adjacent zones.
// They maintain the zone IDs either side of them, and the order is related to the vertex ID order:
// if we imagine a zone perimeter to be defined by edges in clockwise order, then the internal zone
// has the same index as the vertex id corresponding to the first vertex of the clockwise edge.
// The other zone index corresponds to the zone outside the perimeter (or none).
// Note this is reversed in the case of hole edges.
struct edge_t {
    element_id_t vertex_ids[2];
    element_id_t zone_ids[2];
    uint8_t upper_colour;
    uint8_t lower_colour;
};

// Define edge_t array type
#define TEMPLATE_NAME edges
#define TEMPLATE_TYPE edge_t
#include "templates/array.h.template"


void edge_set_zone_by_vertex(edge_t *edge, element_id_t vertex_id, element_id_t zone_id);
element_id_t edge_get_zone_by_vertex(const edge_t *edge, element_id_t vertex_id);
element_id_t edge_get_other_vertex(const edge_t *edge, element_id_t vertex_id);
aabb2f_t edge_get_aabb(const edge_t *edge, vertices_view_t vertices);


#endif // ifndef EDGE_H_
