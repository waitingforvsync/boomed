#ifndef BOOMED_WORLD_SUBZONE_H_
#define BOOMED_WORLD_SUBZONE_H_

#include "boomed/array.h"
#include "boomed/world/element_id.h"

typedef struct subzone_t subzone_t;

// A subzone is just an array of vertex IDs defined in a clockwise order.
struct subzone_t {
    array_t(element_id_t, vertex_ids);
};


#endif // ifndef BOOMED_WORLD_SUBZONE_H_
