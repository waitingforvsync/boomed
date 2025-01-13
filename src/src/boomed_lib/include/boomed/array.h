#ifndef BOOMED_ARRAY_H_
#define BOOMED_ARRAY_H_

#include <stdint.h>
#include <string.h>

#define array_t(type, name) type *name; uint32_t name##_num, name##_capacity

#define array_init(name) (name##_num = 0, name##_capacity = 0, name = 0)
#define array_init_reserve(name, arena, size) (name##_num = 0, name##_capacity = size, name = arena_alloc(arena, size * sizeof(*name)))
#define array_reserve(name, arena, size) ((size > name##_capacity) && (name = arena_realloc(arena, name, name##_capacity * sizeof(*name), size * sizeof(*name)), name##_capacity = size))
#define array_resize(name, arena, size) (name##_num = size, ((size > name##_capacity) && (name = arena_realloc(arena, name, name##_capacity * sizeof(*name), size * sizeof(*name)), name##_capacity = size)))
#define array_reset(name) (name##_num = 0)
#define array_add(name, arena, ...) (((name##_num == name##_capacity) && (name = arena_realloc(arena, name, name##_capacity * sizeof(*name), (name##_num + 1) * 2 * sizeof(*name)), name##_capacity = (name##_num + 1) * 2)), name[name##_num] = __VA_ARGS__, name##_num++)
#define array_pop(name) ((name##_num > 0) && --name##_num)
#define array_back(name) (name[name##_num - 1])
#define array_insert(name, arena, index, ...) ((index) <= name##_num && (((name##_num == name##_capacity) && (name = arena_realloc(arena, name, name##_capacity * sizeof(*name), (name##_num + 1) * 2 * sizeof(*name)), name##_capacity = (name##_num + 1) * 2)), memmove(name + (index) + 1, name + (index), (name##_num - (index)) * sizeof(*name)), name[index] = __VA_ARGS__, name##_num++))
#define array_remove(name, index) // @todo
#define array_is_valid(name) (!!name)
#define array_is_empty(name) (!name##_num)

#define INDEX_NONE (0xFFFFFFFFU)


#endif // ifndef BOOMED_ARRAY_H_
