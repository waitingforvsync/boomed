#ifndef BOOMED_ARRAY_H_
#define BOOMED_ARRAY_H_

#include <stdint.h>

#define array_t(type, name) type *name; uint32_t name##_num, name##_capacity
#define array_init(name, size, arena) name = arena_alloc(arena, size * sizeof(*name)), name##_num = 0, name##_capacity = size
#define array_resize(name, size, arena) name##_num = size
#define array_add(name, ...) (name[name##_num++] = __VA_ARGS__, name##_num)
#define array_is_valid(name) (!!name)
#define array_is_empty(name) (!name##_num)

#define INDEX_NONE (0xFFFFFFFFU)

#endif // ifndef BOOMED_ARRAY_H_
