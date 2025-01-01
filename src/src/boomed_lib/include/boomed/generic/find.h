#ifndef BOOMED_GENERIC_FIND_H_
#define BOOMED_GENERIC_FIND_H_

#include "boomed/generic/slice.h"


#define DEF_FIND(name, type) \
    static inline uint32_t find_##name(slice_const_##name##_t slice, const type *value) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (is_equal_##name(slice.data + i, value) { \
                return i; \
            } \
        } \
        return INDEX_NONE; \
    } \
    \
    static inline uint32_t find_##name##_by_pred_ctx(slice_const_##name##_t slice, bool (*fn)(void *, const type *), void *ctx) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (fn(ctx, slice.data + i)) { \
                return i; \
            } \
        } \
        return INDEX_NONE; \
    }


#endif // ifndef BOOMED_GENERIC_FIND_H_
