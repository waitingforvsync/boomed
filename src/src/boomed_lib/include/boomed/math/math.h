#ifndef BOOMED_MATH_MATH_H_
#define BOOMED_MATH_MATH_H_

#include <stdbool.h>
#include "boomed/defines.h"


static inline float deg_to_rad(float degrees) {
    return degrees * 0.0174532925f;
}

#define DEF_ABS(name, type) \
    static inline type abs_##name(type a) { \
        return (a < 0) ? -a : a; \
    }

DEF_ABS(int8, int8_t)
DEF_ABS(int16, int16_t)
DEF_ABS(int32, int32_t)
DEF_ABS(int64, int64_t)
DEF_ABS(float, float)
DEF_ABS(double, double)

#define DEF_MIN(name, type) \
    static inline type min_##name(type a, type b) { \
        return (a < b) ? a : b; \
    }

DEF_MIN(int8, int8_t)
DEF_MIN(uint8, uint8_t)
DEF_MIN(int16, int16_t)
DEF_MIN(uint16, uint16_t)
DEF_MIN(int32, int32_t)
DEF_MIN(uint32, uint32_t)
DEF_MIN(int64, int64_t)
DEF_MIN(uint64, uint64_t)
DEF_MIN(float, float)
DEF_MIN(double, double)

#define DEF_MAX(name, type) \
    static inline type max_##name(type a, type b) { \
        return (a > b) ? a : b; \
    }

DEF_MAX(int8, int8_t)
DEF_MAX(uint8, uint8_t)
DEF_MAX(int16, int16_t)
DEF_MAX(uint16, uint16_t)
DEF_MAX(int32, int32_t)
DEF_MAX(uint32, uint32_t)
DEF_MAX(int64, int64_t)
DEF_MAX(uint64, uint64_t)
DEF_MAX(float, float)
DEF_MAX(double, double)


static inline int32_t sgn_int32(int32_t a) {
    return (a < 0) ? -1 : (a > 0) ? 1 : 0;
}

static inline int64_t sgn_int64(int64_t a) {
    return (a < 0) ? -1 : (a > 0) ? 1 : 0;
}

static inline int64_t gcd_int64(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return abs_int64(a);
}

static inline uint32_t clz_uint32(uint32_t a) {
    assert(a != 0);
#ifdef _MSC_VER
    return __lzcnt(a);
#else
    return __builtin_clz(a);
#endif
}

static inline uint32_t clz_uint64(uint64_t a) {
    assert(a != 0);
#ifdef _MSC_VER
    return __lzcnt64(a);
#else
    return __builtin_clzll(a);
#endif
}

static inline bool does_mul_overflow_uint64(uint64_t a, uint64_t b) {
    return a != 0 && b > UINT64_MAX / a;
}

static inline bool does_add_overflow_uint64(uint64_t a, uint64_t b) {
    return b > UINT64_MAX - a;
}



#endif // ifndef BOOMED_MATH_MATH_H_
