#ifndef WORLD_CONTOUR_H_
#define WORLD_CONTOUR_H_

#include <stdbool.h>
#include "base/array.h"
#include "world/element_id.h"

typedef struct contour_t contour_t;
typedef struct vertices_view_t vertices_view_t;
typedef struct edges_view_t edges_view_t;
typedef struct arena_t arena_t;


// A contour is just an array of edge IDs.
// Contours are used to define zone perimeters and also any holes which may be present within a zone.
// A perimeter contour is defined in clockwise order, while a hole contour is defined in counter-clockwise order.
struct contour_t {
    array_t(element_id_t, edge_ids);
};

// Define contour_t array type
#define TEMPLATE_NAME contours
#define TEMPLATE_TYPE contour_t
#include "templates/array.h.template"


contour_t contour_make(vertices_view_t vertices, edges_view_t edges, element_id_t edge_id, element_id_t start_vertex_id, arena_t *arena);
contour_t contour_make_copy(const contour_t *contour_to_copy, arena_t *arena);

bool contour_is_valid(const contour_t *contour);
element_id_t contour_get_start_vertex(const contour_t *contour, edges_view_t edges);
const element_id_t *contour_get_vertices(const contour_t *contour, edges_view_t edges, arena_t *arena);
int32_t contour_get_winding(const contour_t *contour, vertices_view_t vertices, edges_view_t edges);
bool contour_is_perimeter(const contour_t *contour, vertices_view_t vertices, edges_view_t edges);
bool contour_is_hole(const contour_t *contour, vertices_view_t vertices, edges_view_t edges);


#endif // ifndef WORLD_CONTOUR_H_
