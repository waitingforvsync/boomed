#ifndef BOOMED_GENERIC_IS_LESS_H_
#define BOOMED_GENERIC_IS_LESS_H_

#include <stdbool.h>
#include <stdint.h>


#define DEF_IS_LESS(name, type) \
    static inline bool is_less_##name(const type *a, const type *b) { \
        return *a < *b; \
    }

DEF_IS_LESS(int32, int32_t)
DEF_IS_LESS(uint32, uint32_t)
DEF_IS_LESS(int64, int64_t)
DEF_IS_LESS(uint64, uint64_t)
DEF_IS_LESS(float, float)
DEF_IS_LESS(double, double)


#endif // ifndef BOOMED_GENERIC_IS_LESS_H_
