#ifndef MAIN_SDL3_H_
#define MAIN_SDL3_H_

#include <stdint.h>

typedef struct vec2f_t vec2f_t;


void draw_polygon(const vec2f_t *points, uint32_t num_points, uint32_t color);
void draw_point(vec2f_t centre, float radius, uint32_t color);
void draw_thick_line(vec2f_t start, vec2f_t end, float width, uint32_t color);


#endif // ifndef MAIN_SDL3_H_
