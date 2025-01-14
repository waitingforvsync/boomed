#ifndef BOOMED_ARRAY_H_
#define BOOMED_ARRAY_H_

#include <stdint.h>
#include <string.h>
#include "boomed/arena.h"

#define array_t(type, name) type *name; uint32_t name##_num, name##_capacity

#define array_init(name) (name##_num = 0, name##_capacity = 0, name = 0)
#define array_init_reserve(name, arena, size) (name##_num = 0, name##_capacity = (size), name = arena_alloc(arena, (size) * sizeof(*name)))
#define array_init_copy(name, arena, src) (array_init_reserve(name, arena, src##_num), memcpy(name, src, src##_num * sizeof(*name)), name##_num = src##_num)
#define array_init_copy_reserve(name, arena, src, size) (array_init_reserve(name, arena, size), memcpy(name, src, src##_num * sizeof(*name)), name##_num = src##_num)
#define array_grow(name, arena, min_size, size) (((min_size) > name##_capacity) && (name = arena_realloc(arena, name, name##_capacity * sizeof(*name), (size) * sizeof(*name)), name##_capacity = (size)))
#define array_reserve(name, arena, size) array_grow(name, arena, size, size)
#define array_resize(name, arena, size) (array_reserve(name, arena, size), name##_num = size)
#define array_reset(name) (name##_num = 0)
#define array_add(name, arena, ...) (array_grow(name, arena, name##_num + 1, (name##_num + 1) * 2), name[name##_num] = (__VA_ARGS__), name##_num++)
#define array_pop(name) ((name##_num > 0) && --name##_num)
#define array_get_first(name) (name[0])
#define array_get_last(name) (name[name##_num - 1])
#define array_insert(name, arena, index, ...) (((index) <= name##_num) && (array_grow(name, arena, name##_num + 1, (name##_num + 1) * 2), memmove(name + (index) + 1, name + (index), (name##_num - (index)) * sizeof(*name)), name[index] = (__VA_ARGS__), ++name##_num))
#define array_remove(name, index) (((index) <= name##_num) && (memmove(name + (index), name + (index) + 1, (name##_num - (index) - 1) * sizeof(*name)), --name##_num))
#define array_is_valid(name) (!!name)
#define array_is_empty(name) (!name##_num)

#define INDEX_NONE (0xFFFFFFFFU)


#endif // ifndef BOOMED_ARRAY_H_
