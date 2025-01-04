#ifndef BOOMED_GENERIC_FOR_EACH_H_
#define BOOMED_GENERIC_FOR_EACH_H_

#include "boomed/generic/slice.h"


#define DEF_FOR_EACH(name, type) \
    static inline void for_each_##name(slice_##name##_t slice, void (*fn)(uint32_t, type *)) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(i, slice.data + i); \
        } \
    } \
    \
    static inline void for_each_##name##_ctx(slice_##name##_t slice, void (*fn)(void *, uint32_t, type *), void *ctx) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(ctx, i, slice.data + i); \
        } \
    } \
    \
    static inline void for_each_const_##name(slice_const_##name##_t slice, void (*fn)(uint32_t, const type *)) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(i, slice.data + i); \
        } \
    } \
    \
    static inline void for_each_const_##name##_ctx(slice_const_##name##_t slice, void (*fn)(const void *, uint32_t, const type *), const void *ctx) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            fn(ctx, i, slice.data + i); \
        } \
    }


#endif // ifndef BOOMED_GENERIC_FOR_EACH_H_
