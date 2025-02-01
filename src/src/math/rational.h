#ifndef BOOMED_MATH_RATIONAL_H_
#define BOOMED_MATH_RATIONAL_H_

#include "math/math.h"


typedef struct rational_t rational_t;

struct rational_t {
    int64_t num;
    int64_t denom;
};



static inline rational_t make_rational(int64_t num, int64_t denom) {
    if (denom == 0) {
        return (rational_t){0};
    }
    if (denom < 0) {
        num = -num;
        denom = -denom;
    }
    int64_t d = gcd_int64(num, denom);
    return (rational_t){
        num / d,
        denom / d
    };
}

static inline rational_t make_rational_from_int(int64_t num) {
    return (rational_t) {num, 1};
}

static inline bool rational_is_valid(rational_t a) {
    return a.denom > 0 && gcd_int64(a.num, a.denom) == 1;
}

static inline rational_t rational_int_mul(rational_t a, int64_t b) {
    return make_rational(a.num * b, a.denom);
}

static inline rational_t rational_mul(rational_t a, rational_t b) {
    return make_rational(a.num * b.num, a.denom * b.denom);
}

static inline rational_t rational_int_div(rational_t a, int64_t b) {
    return make_rational(a.num * sgn_int64(b), a.denom * b);
}

static inline rational_t rational_div(rational_t a, rational_t b) {
    return make_rational(a.num * b.denom * sgn_int64(b.num), a.denom * abs_int64(b.num));
}

static inline rational_t rational_int_add(rational_t a, int64_t b) {
    return (rational_t) {
        a.num + b * a.denom,
        a.denom
    };
}

static inline rational_t rational_add(rational_t a, rational_t b) {
    int64_t d = gcd_int64(a.denom, b.denom);
    int64_t da = a.denom / d;
    int64_t db = b.denom / d;
    return (rational_t) {
        a.num * db + b.num * da,
        da * b.denom
    };
}

static inline rational_t rational_int_sub(rational_t a, int64_t b) {
    return (rational_t) {
        a.num - b * a.denom,
        a.denom
    };
}

static inline rational_t rational_sub(rational_t a, rational_t b) {
    int64_t d = gcd_int64(a.denom, b.denom);
    int64_t da = a.denom / d;
    int64_t db = b.denom / d;
    return (rational_t) {
        a.num * db - b.num * da,
        da * b.denom
    };
}



#endif // ifndef MATH_RATIONAL_H_
