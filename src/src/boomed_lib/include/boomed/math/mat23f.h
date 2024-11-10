#ifndef BOOMED_MATH_MAT23F_H_
#define BOOMED_MATH_MAT23F_H_

#include "boomed/math/mat22f.h"


typedef struct mat23f_t mat23f_t;

struct mat23f_t {
    mat22f_t m;
    vec2f_t t;
};


static inline mat23f_t mat23f_make_identity(void) {
	return (mat23f_t){
		mat22f_make_identity(),
		vec2f_make_zero()
	};
}

static inline mat23f_t mat23f_make_from_floats(const float *f) {
	return (mat23f_t){
		mat22f_make_from_floats(f),
		vec2f_make_from_floats(f+4)
	};
}

static inline vec2f_t mat23f_vec2f_mul(mat23f_t m, vec2f_t v) {
	return (vec2f_t){
		m.m.x.x * v.x + m.m.y.x * v.y + m.t.x,
		m.m.x.y * v.x + m.m.y.y * v.y + m.t.y
    };
}

static inline mat23f_t mat23f_mul(mat23f_t a, mat23f_t b) {
	return (mat23f_t){
		mat22f_mul(a.m, b.m),
		mat23f_vec2f_mul(a, b.t)
	};
}

static inline mat23f_t mat23f_inverse(mat23f_t m) {
	mat22f_t mi = mat22f_inverse(m.m);
	return (mat23f_t){
		mi,
		mat22f_vec2f_mul(mi, vec2f_negate(m.t))
	};
}


#endif // ifndef BOOMED_MATH_MAT23F_H_
