#ifndef BOOMED_MATH_MATH_H_
#define BOOMED_MATH_MATH_H_

#include <stdbool.h>
#include <stdint.h>


static inline float deg_to_rad(float degrees) {
    return degrees * 0.0174532925f;
}

#define DEF_ABS(name, type) \
    static inline type abs_##name(type a) { \
        return (a < 0) ? -a : a; \
    }

DEF_ABS(int32, int32_t)
DEF_ABS(int64, int64_t)
DEF_ABS(float, float)
DEF_ABS(double, double)


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

static inline bool does_mul_overflow_uint64(uint64_t a, uint64_t b) {
    return a != 0 && b > UINT64_MAX / a;
}

static inline bool does_add_overflow_uint64(uint64_t a, uint64_t b) {
    return b > UINT64_MAX - a;
}



#endif // ifndef BOOMED_MATH_MATH_H_
