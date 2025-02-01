#ifndef MATH_MAT22F_H_
#define MATH_MAT22F_H_

#include "math/vec2f.h"


typedef struct mat22f_t mat22f_t;

struct mat22f_t {
    vec2f_t x, y;
};


static inline mat22f_t mat22f_make_zero(void) {
	return (mat22f_t) {
		vec2f_make_zero(),
		vec2f_make_zero()
	};
}

static inline mat22f_t mat22f_make_identity(void) {
	return (mat22f_t) {
		vec2f_make_unitx(),
		vec2f_make_unity()
	};
}

static inline mat22f_t mat22f_make_rot(float angle) {
	float s = sinf(angle);
	float c = cosf(angle);
	return (mat22f_t) {
		{ c, s},
		{-s, c}
	};
}

static inline mat22f_t mat22f_make_from_floats(const float *f) {
	return (mat22f_t){
		vec2f_make_from_floats(f),
		vec2f_make_from_floats(f+2)
	};
}

static inline mat22f_t mat22f_add(mat22f_t a, mat22f_t b) {
    return (mat22f_t) {
        vec2f_add(a.x, b.x),
        vec2f_add(a.y, b.y)
    };
}

static inline mat22f_t mat22f_sub(mat22f_t a, mat22f_t b) {
    return (mat22f_t) {
        vec2f_sub(a.x, b.x),
        vec2f_sub(a.y, b.y)
    };
}

static inline mat22f_t mat22f_scalar_mul(mat22f_t m, float s) {
	return (mat22f_t) {
		vec2f_scalar_mul(m.x, s),
		vec2f_scalar_mul(m.y, s)
	};
}

static inline vec2f_t mat22f_vec2f_mul(mat22f_t m, vec2f_t v) {
	return (vec2f_t) {
		m.x.x * v.x + m.y.x * v.y,
		m.x.y * v.x + m.y.y * v.y
    };
}

static inline mat22f_t mat22f_mul(mat22f_t a, mat22f_t b) {
	return (mat22f_t) {
		mat22f_vec2f_mul(a, b.x),
		mat22f_vec2f_mul(a, b.y)
	};
}

static inline float mat22f_determinant(mat22f_t a) {
	return a.x.x * a.y.y - a.x.y * a.y.x;
}

static inline mat22f_t mat22f_transpose(mat22f_t a) {
	return (mat22f_t) {
		{a.x.x, a.y.x},
		{a.x.y, a.y.y}
	};
}

static inline mat22f_t mat22f_inverse(mat22f_t a) {
    float det = mat22f_determinant(a);
    assert(det != 0.0f);
	float d = 1.0f / det;
	return (mat22f_t) {
		{ a.y.y * d, -a.x.y * d},
		{-a.y.x * d,  a.x.x * d}
	};
}



#endif // ifndef MATH_MAT22F_H_
