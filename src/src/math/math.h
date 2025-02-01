#ifndef MATH_H_
#define MATH_H_

#include "base/defines.h"


static inline float deg_to_rad(float degrees) {
    return degrees * 0.0174532925f;
}

#define DEF_ABS_(name, type) \
    static inline type abs_##name(type a) { \
        return (a < 0) ? -a : a; \
    }

DEF_ABS_(int8, int8_t)
DEF_ABS_(int16, int16_t)
DEF_ABS_(int32, int32_t)
DEF_ABS_(int64, int64_t)
DEF_ABS_(float, float)
DEF_ABS_(double, double)
#undef DEF_ABS_

#define DEF_MIN_(name, type) \
    static inline type min_##name(type a, type b) { \
        return (a < b) ? a : b; \
    }

DEF_MIN_(int8, int8_t)
DEF_MIN_(uint8, uint8_t)
DEF_MIN_(int16, int16_t)
DEF_MIN_(uint16, uint16_t)
DEF_MIN_(int32, int32_t)
DEF_MIN_(uint32, uint32_t)
DEF_MIN_(int64, int64_t)
DEF_MIN_(uint64, uint64_t)
DEF_MIN_(float, float)
DEF_MIN_(double, double)
#undef DEF_MIN_

#define DEF_MAX_(name, type) \
    static inline type max_##name(type a, type b) { \
        return (a > b) ? a : b; \
    }

DEF_MAX_(int8, int8_t)
DEF_MAX_(uint8, uint8_t)
DEF_MAX_(int16, int16_t)
DEF_MAX_(uint16, uint16_t)
DEF_MAX_(int32, int32_t)
DEF_MAX_(uint32, uint32_t)
DEF_MAX_(int64, int64_t)
DEF_MAX_(uint64, uint64_t)
DEF_MAX_(float, float)
DEF_MAX_(double, double)
#undef DEF_MAX_

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
    return (uint32_t)__lzcnt64(a);
#else
    return (uint32_t)__builtin_clzll(a);
#endif
}

static inline bool does_mul_overflow_uint64(uint64_t a, uint64_t b) {
    return a != 0 && b > UINT64_MAX / a;
}

static inline bool does_add_overflow_uint64(uint64_t a, uint64_t b) {
    return b > UINT64_MAX - a;
}



#endif // ifndef MATH_H_
