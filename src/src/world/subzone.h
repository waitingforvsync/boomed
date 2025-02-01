#ifndef WORLD_SUBZONE_H_
#define WORLD_SUBZONE_H_

#include "base/array.h"
#include "world/element_id.h"

typedef struct subzone_t subzone_t;

// A subzone is just an array of vertex IDs defined in a clockwise order.
struct subzone_t {
    array_t(element_id_t, vertex_ids);
};

// Define subzone_t array type
#define TEMPLATE_NAME subzones
#define TEMPLATE_TYPE subzone_t
#include "templates/array.h.template"


#endif // ifndef WORLD_SUBZONE_H_
