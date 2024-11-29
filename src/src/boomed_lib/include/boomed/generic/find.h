#ifndef BOOMED_GENERIC_FIND_H_
#define BOOMED_GENERIC_FIND_H_

#include "boomed/generic/slice.h"


#define DEF_FIND(name, type) \
    static inline uint32_t find_##name(slice_##name##_t slice, type value) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (is_equal_##name(slice.data[i], value) { \
                return i; \
            } \
        } \
        return INDEX_NONE; \
    } \
    static inline uint32_t find_##name##_by_pred_ctx(slice_##name##_t slice, bool (*fn)(const type *, void *), void *context) { \
        for (uint32_t i = 0; i < slice.size; i++) { \
            if (fn(slice.data[i], context)) { \
                return i; \
            } \
        } \
        return INDEX_NONE; \
    }


#endif // ifndef BOOMED_GENERIC_FIND_H_
