#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "boomed/math/mat23f.h"


typedef struct viewport_t viewport_t;

struct viewport_t {
    mat23f_t transform;
    vec2f_t size;
    vec2f_t viewport_centre;
    vec2f_t click_initial_pos;
    float viewport_zoom;
    bool edge_mode;
};


void viewport_init(viewport_t *viewport);


#endif // VIEWPORT_H_
