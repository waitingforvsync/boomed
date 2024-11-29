#ifndef BOOMED_GENERIC_FOR_EACH_H_
#define BOOMED_GENERIC_FOR_EACH_H_

#include "boomed/generic/slice.h"


#define DEF_FOR_EACH(name, type) \
    static inline void for_each_##name(slice_##name##_t slice, void (*fn)(uint32_t, type *)) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(i, slice.data[i]); \
        } \
    } \
    static inline void for_each_##name##_ctx(slice_##name##_t slice, void (*fn)(uint32_t, type *, void *), void *context) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(i, slice.data[i], context); \
        } \
    }


#endif // ifndef BOOMED_GENERIC_FOR_EACH_H_
