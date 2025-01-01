#ifndef BOOMED_GENERIC_ALL_OF_H_
#define BOOMED_GENERIC_ALL_OF_H_

#include <stdbool.h>
#include <stdint.h>


#define DEF_ALL_OF(name, type) \
    static inline bool all_of_##name(slice_const_##name##_t slice, bool (*fn)(const type *)) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (!fn(slice.data + i)) { \
                return false; \
            } \
        } \
        return true; \
    } \
    \
    static inline bool all_of_##name##_ctx(slice_const_##name##_t slice, bool (*fn)(void *, const type *), void *ctx) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (!fn(ctx, slice.data + i)) { \
                return false; \
            } \
        } \
        return true; \
    }


#endif // ifndef BOOMED_GENERIC_ALL_OF_H_
