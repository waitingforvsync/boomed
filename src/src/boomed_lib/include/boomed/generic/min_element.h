#ifndef BOOMED_GENERIC_MIN_ELEMENT_H_
#define BOOMED_GENERIC_MIN_ELEMENT_H_

#include "boomed/generic/slice.h"


#define DEF_MIN_ELEMENT_BY_VALUE(name, type) \
    static inline uint32_t min_element_##name(slice_const_##name##_t slice, type value) { \
        uint32_t result = 0; \
        for (uint32_t i = 1; i < slice.size; i++) { \
            if (slice.data[result] < slice.data[i]) { \
                result = i; \
            } \
        } \
        return result; \
    }

#define DEF_MIN_ELEMENT_BY_SCORE(name, score_name, element_type, score_type) \
    static inline uint32_t min_element_##name##_by_##score_name(slice_const_##name##_t slice, score_type (*score_fn)(const element_type *), score_type initial_score) { \
        uint32_t result = INDEX_NONE; \
        for (uint32_t i = 0; i < slice.size; i++) { \
            score_type score = score_fn(slice.data + i); \
            if (is_less_##score_name(&score, &initial_score)) { \
                initial_score = score; \
                result = i; \
            } \
        } \
        return result; \
    }

#define DEF_MIN_ELEMENT_BY_SCORE_CTX(name, score_name, element_type, score_type) \
    static inline uint32_t min_element_##name##_by_##score_name##_ctx(slice_const_##name##_t slice, score_type (*score_fn)(void *, const element_type *), score_type initial_score, void *ctx) { \
        uint32_t result = INDEX_NONE; \
        for (uint32_t i = 0; i < slice.size; i++) { \
            score_type score = score_fn(ctx, slice.data + i); \
            if (is_less_##score_name(&score, &initial_score)) { \
                initial_score = score; \
                result = i; \
            } \
        } \
        return result; \
    }


#endif // ifndef BOOMED_GENERIC_MIN_ELEMENT_H_
