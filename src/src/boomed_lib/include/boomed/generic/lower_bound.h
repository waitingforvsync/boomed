#ifndef BOOMED_GENERIC_LOWER_BOUND_H_
#define BOOMED_GENERIC_LOWER_BOUND_H_

#include "boomed/generic/slice.h"


#define DEF_LOWER_BOUND(name, type) \
    static inline uint32_t lower_bound_##name(slice_const_##name##_t slice, const type *value) { \
        uint32_t index = 0; \
        for (uint32_t count = slice.size; count > 0; count /= 2) { \
            if (is_less_##name(slice.data + index + count / 2, value)) index += count-- / 2 + 1; \
        } \
        return index; \
    } \
    \
    static inline uint32_t lower_bound_##name##_by_pred(slice_const_##name##_t slice, const type *value, bool (*fn)(const type *, const type *)) { \
        uint32_t index = 0; \
        for (uint32_t count = slice.size; count > 0; count /= 2) { \
            if (fn(slice.data + index + count / 2, value)) index += count-- / 2 + 1; \
        } \
        return index; \
    } \
    \
    static inline uint32_t lower_bound_##name##_by_pred_ctx(slice_const_##name##_t slice, const type *value, bool (*fn)(void *, const type *, const type *), void *ctx) { \
        uint32_t index = 0; \
        for (uint32_t count = slice.size; count > 0; count /= 2) { \
            if (fn(ctx, slice.data + index + count / 2, value)) index += count-- / 2 + 1; \
        } \
        return index; \
    }


#endif // ifndef BOOMED_GENERIC_LOWER_BOUND_H_
