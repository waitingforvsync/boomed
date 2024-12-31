#ifndef BOOMED_GENERIC_MIN_H_
#define BOOMED_GENERIC_MIN_H_

#include <stdint.h>


#define DEF_MIN(name, type) \
    static inline type min_##name(type a, type b) { \
        return (a < b) ? a : b; \
    }

DEF_MIN(int32, int32_t)
DEF_MIN(uint32, uint32_t)
DEF_MIN(int64, int64_t)
DEF_MIN(uint64, uint64_t)
DEF_MIN(float, float)
DEF_MIN(double, double)


#endif // ifndef BOOMED_GENERIC_MIN_H_
