#ifndef BOOMED_GENERIC_SLICE_H_
#define BOOMED_GENERIC_SLICE_H_

#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#define DEF_SLICE(name, type) \
    typedef struct slice_const_##name##_t { \
        const type *data; \
        uint32_t size; \
    } slice_const_##name##_t; \
    \
    typedef union slice_##name##_t { \
        struct { \
            type *data; \
            uint32_t size; \
        }; \
        slice_const_##name##_t const_slice; \
    } slice_##name##_t;


#define DEF_IS_EQUAL_SLICE(name, type) \
    static inline bool is_equal_slice_##name(slice_const_##name##_t a, slice_const_##name##_t b) { \
        return a.size == b.size && memcmp(a.data, b.data, a.size) == 0; \
    }


#define SLICE(type, ...) { \
        .data = (type[]){__VA_ARGS__}, \
        .num = sizeof((type[]){__VA_ARGS__}) / sizeof(type) \
    }


#define INDEX_NONE (0xFFFFFFFFU)


#endif // ifndef BOOMED_GENERIC_SLICE_H_
