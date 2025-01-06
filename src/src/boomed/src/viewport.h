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
    vec2f_t action_initial_world_pos;
    float zoom;
    bool is_panning;
    bool is_dragging;
};


void viewport_init(viewport_t *viewport);
void viewport_resize(viewport_t *viewport, vec2f_t size);
void viewport_pan_start(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_pan_move(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_pan_stop(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_action_start(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_action_move(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_action_stop(viewport_t *viewport, vec2f_t viewport_pos);
void viewport_set_zoom(viewport_t *viewport, vec2f_t viewport_pos, float zoom_delta);
void viewport_command_undo(viewport_t *viewport);
void viewport_command_redo(viewport_t *viewport);
void viewport_command_cut(viewport_t *viewport);
void viewport_command_copy(viewport_t *viewport);
void viewport_command_paste(viewport_t *viewport);
void viewport_render(const viewport_t *render);


#endif // VIEWPORT_H_
