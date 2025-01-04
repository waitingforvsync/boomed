#ifndef BOOMED_GENERIC_EQUAL_RANGE_H_
#define BOOMED_GENERIC_EQUAL_RANGE_H_

#include "boomed/generic/lower_bound.h"
#include "boomed/generic/upper_bound.h"
#include "boomed/generic/pair.h"


#define DEF_EQUAL_RANGE(name, type) \
    static inline pair_uint32_t equal_range_##name(slice_const_##name##_t slice, const type *value) { \
        return (pair_uint32_t){ \
            lower_bound_##name(slice, value), \
            upper_bound_##name(slice, value) \
        }; \
    } \
    \
    static inline pair_uint32_t equal_range_##name##_by_pred(slice_const_##name##_t slice, const type *value, bool (*fn)(const type *, const type *)) { \
        return (pair_uint32_t){ \
            lower_bound_##name##_by_pred(slice, value, fn), \
            upper_bound_##name##_by_pred(slice, value, fn) \
        }; \
    } \
    \
    static inline pair_uint32_t equal_range_##name##_by_pred_ctx(slice_const_##name##_t slice, const type *value, bool (*fn)(void *, const type *, const type *), void *ctx) { \
        return (pair_uint32_t){ \
            lower_bound_##name##_by_pred_ctx(slice, value, fn, ctx), \
            upper_bound_##name##_by_pred_ctx(slice, value, fn, ctx) \
        }; \
    }


#endif // ifndef BOOMED_GENERIC_EQUAL_RANGE_H_
