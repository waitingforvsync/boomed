#ifndef BOOMED_MATH_MATH_H_
#define BOOMED_MATH_MATH_H_

#include <stdbool.h>
#include <stdint.h>


static inline float deg_to_rad(float degrees) {
    return degrees * 0.0174532925f;
}

static inline int32_t abs_int32(int32_t a) {
    return (a < 0) ? -a : a;
}

static inline int64_t abs_int64(int64_t a) {
    return (a < 0) ? -a : a;
}

static inline int32_t sgn_int32(int32_t a) {
    return (a < 0) ? -1 : (a > 0) ? 1 : 0;
}

static inline int64_t sgn_int64(int64_t a) {
    return (a < 0) ? -1 : (a > 0) ? 1 : 0;
}

static inline int64_t gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t t = b;
        b = a % b;
        a = t;
    }
    return abs_int64(a);
}

static inline bool mul_overflow_uint64(uint64_t a, uint64_t b) {
    return a != 0 && b > UINT64_MAX / a;
}

static inline bool add_overflow_uint64(uint64_t a, uint64_t b) {
    return b > UINT64_MAX - a;
}



#endif // ifndef BOOMED_MATH_MATH_H_
