#ifndef BOOMED_GENERIC_ARRAY_H_
#define BOOMED_GENERIC_ARRAY_H_

#include <string.h>
#include "boomed/generic/slice.h"

typedef struct arena_t arena_t;


#define DEF_ARRAY(name, type) \
    DEF_SLICE(name, type) \
    typedef struct array_##name##_t { \
        union { \
            struct { \
                type *data; \
                uint32_t size; \
            }; \
            slice_##name##_t slice; \
            slice_const_##name##_t const_slice; \
        }; \
        uint32_t capacity; \
    } array_##name##_t; \
    \
    array_##name##_t make_array_##name(arena_t *arena, uint32_t initial_capacity); \
    array_##name##_t copy_array_##name(arena_t *arena, const array_##name##_t *src_array, uint32_t initial_capacity); \
    void array_##name##_init(array_##name##_t *array, arena_t *arena, uint32_t initial_capacity); \
    void array_##name##_deinit(array_##name##_t *array, arena_t *arena); \
    void array_##name##_reserve(array_##name##_t *array, arena_t *arena, uint32_t capacity); \
    void array_##name##_resize(array_##name##_t *array, arena_t *arena, uint32_t size); \
    void array_##name##_resize_clear(array_##name##_t *array, arena_t *arena, uint32_t size); \
    void array_##name##_insert(array_##name##_t *array, arena_t *arena, uint32_t at, uint32_t num); \
    void array_##name##_remove(array_##name##_t *array, uint32_t at, uint32_t num); \
    void array_##name##_append(array_##name##_t *array, arena_t *arena, const array_##name##_t *src_array);


#define DEF_ARRAY_IMPL(name, type) \
    array_##name##_t make_array_##name(arena_t *arena, uint32_t initial_capacity) { \
        return (array_##name##_t) { \
            .data = arena_alloc(arena, initial_capacity * sizeof(type)), \
            .size = 0, \
            .capacity = initial_capacity \
        }; \
    } \
    \
    array_##name##_t copy_array_##name(arena_t *arena, const array_##name##_t *src_array, uint32_t initial_capacity) { \
        if (initial_capacity == 0) { \
            initial_capacity = src_array->size; \
        } \
        type *data = arena_alloc(arena, initial_capacity * sizeof(type)); \
        memcpy(data, src_array->data, src_array->size * sizeof(type)); \
        return (array_##name##_t) { \
            .data = data, \
            .size = src_array->size, \
            .capacity = initial_capacity \
        }; \
    } \
    \
    void array_##name##_init(array_##name##_t *array, arena_t *arena, uint32_t initial_capacity) { \
        array->data = arena_realloc(arena, array->data, initial_capacity); \
        array->size = 0; \
        array->capacity = initial_capacity; \
    } \
    \
    void array_##name##_deinit(array_##name##_t *array, arena_t *arena) { \
        arena_free(arena, array->data); \
        array->data = 0; \
        array->size = 0; \
        array->capacity = 0; \
    } \
    \
    void array_##name##_reserve(array_##name##_t *array, arena_t *arena, uint32_t capacity) { \
        if (capacity > array->capacity) { \
            array->data = arena_realloc(arena, array->data, capacity); \
            array->capacity = capacity; \
        } \
    } \
    \
    void array_##name##_resize(array_##name##_t *array, arena_t *arena, uint32_t size) { \
        if (size > array->size) { \
            array_##name##_reserve(array, arena, size); \
        } \
        array->size = size; \
    } \
    \
    void array_##name##_resize_clear(array_##name##_t *array, arena_t *arena, uint32_t size) { \
        if (size > array->size) { \
            array_##name##_reserve(array, arena, size); \
            memset(array->data + array->size, 0, (size - array->size) * sizeof(type)); \
        } \
        array->size = size; \
    } \
    \
    void array_##name##_insert(array_##name##_t *array, arena_t *arena, uint32_t at, uint32_t num) { \
        if (at > array->size) { \
            num += (at - array->size); \
            at = array->size; \
        } \
        array_##name##_reserve(array, arena, array->size + num); \
        memmove(array->data + at + num, array->data + at, (array->size - at) * sizeof(type)); \
        array->size += num; \
    } \
    \
    void array_##name##_remove(array_##name##_t *array, uint32_t at, uint32_t num) { \
        if (at < array->size) { \
            if (at + num > array->size) { \
                num = array->size - at; \
            } \
            memmove(array->data + at, array->data + at + num, (array->size - at - num) * sizeof(type)); \
            array->size -= num; \
        } \
    } \
    \
    void array_##name##_append(array_##name##_t *array, arena_t *arena, const array_##name##_t *src_array) { \
        array_##name##_reserve(array, arena, array->size + src_array->size); \
        memcpy(array->data + array->size, src_array->data, src_array->size * sizeof(type)); \
        array->size += src_array->size; \
    }


#endif // ifndef BOOMED_GENERIC_SLICE_H_
