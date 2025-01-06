#ifndef BOOMED_GENERIC_OPTIONAL_H_
#define BOOMED_GENERIC_OPTIONAL_H_

#include <stdbool.h>
#include <stdint.h>


#define DEF_OPTIONAL(name, type) \
    typedef struct optional_##name##_t { \
        bool is_valid; \
        type value; \
    } optional_##name##_t;

DEF_OPTIONAL(int32, int32_t)
DEF_OPTIONAL(uint32, uint32_t)
DEF_OPTIONAL(int64, int64_t)
DEF_OPTIONAL(uint64, uint64_t)
DEF_OPTIONAL(float, float)
DEF_OPTIONAL(double, double)


#define DEF_IS_LESS_OPTIONAL(name, type) \
    static inline bool is_less_optional_##name(const optional_##name##_t *a, const optional_##name##_t *b) { \
        return a->is_valid && (!b->is_valid || a->value < b->value); \
    }



#endif // ifndef BOOMED_GENERIC_OPTIONAL_H_
