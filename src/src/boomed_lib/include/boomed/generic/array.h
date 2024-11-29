#ifndef BOOMED_GENERIC_ARRAY_H_
#define BOOMED_GENERIC_ARRAY_H_

#include "boomed/generic/slice.h"


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
    } array_##name##_t;


#endif // ifndef BOOMED_GENERIC_SLICE_H_
