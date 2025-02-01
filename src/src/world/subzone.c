#include "world/subzone.h"


subzone_t subzone_make(arena_t *arena) {
    assert(arena);

    return (subzone_t) {
        .vertex_ids = element_ids_make(arena, 32)
    };
}
