#ifndef SUBZONE_H_
#define SUBZONE_H_

#include "world/element_id.h"

typedef struct subzone_t subzone_t;
typedef struct arena_t arena_t;

// A subzone is just an array of vertex IDs defined in a clockwise order.
struct subzone_t {
    element_ids_t vertex_ids;
};


subzone_t subzone_make(arena_t *arena);


// Define subzone_t array type
#define TEMPLATE_NAME subzones
#define TEMPLATE_TYPE subzone_t
#include "templates/array.h.template"


#endif // ifndef SUBZONE_H_
