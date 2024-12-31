#ifndef BOOMED_GENERIC_MAX_H_
#define BOOMED_GENERIC_MAX_H_

#include <stdint.h>


#define DEF_MAX(name, type) \
    static inline type max_##name(type a, type b) { \
        return (a > b) ? a : b; \
    }

DEF_MAX(int32, int32_t)
DEF_MAX(uint32, uint32_t)
DEF_MAX(int64, int64_t)
DEF_MAX(uint64, uint64_t)
DEF_MAX(float, float)
DEF_MAX(double, double)


#endif // ifndef BOOMED_GENERIC_MAX_H_
