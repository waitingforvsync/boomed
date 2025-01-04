#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "boomed/math/mat23f.h"


typedef struct viewport_t viewport_t;
typedef struct world_t world_t;

struct viewport_t {
    world_t *world;
    mat23f_t world_to_viewport;
    mat23f_t viewport_to_world;
    vec2f_t size;
    vec2f_t focus_world_pos;
    vec2f_t pan_initial_viewport_pos;
    vec2f_t pan_initial_world_pos;
    float zoom;
    bool edge_mode;
    bool is_panning;
};


void viewport_init(viewport_t *viewport);
void viewport_resize(viewport_t *viewport, vec2f_t size);
void viewport_start_pan(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_set_pan(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_stop_pan(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_set_zoom(viewport_t *viewport, vec2f_t viewport_pos, float zoom_delta);
void viewport_render(const viewport_t *render);


#endif // VIEWPORT_H_
