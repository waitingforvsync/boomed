#ifndef BOOMED_GENERIC_ANY_OF_H_
#define BOOMED_GENERIC_ANY_OF_H_

#include <stdbool.h>
#include <stdint.h>


#define DEF_ANY_OF(name, type) \
    static inline bool any_of_##name(slice_const_##name##_t slice, bool (*fn)(const type *)) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (fn(slice.data + i)) { \
                return true; \
            } \
        } \
        return false; \
    } \
    \
    static inline bool any_of_##name##_ctx(slice_const_##name##_t slice, bool (*fn)(void *, const type *), void *ctx) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (fn(ctx, slice.data + i)) { \
                return true; \
            } \
        } \
        return false; \
    }


#endif // ifndef BOOMED_GENERIC_ANY_OF_H_
