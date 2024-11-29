#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "boomed/math/mat23f.h"


typedef struct viewport_t viewport_t;

struct viewport_t {
    mat23f_t transform;
    vec2f_t size;
    vec2f_t initial_pos;
    bool edge_mode;

};


#endif // VIEWPORT_H_
