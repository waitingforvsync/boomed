#include "viewport.h"
#include "main_sdl3.h"
#include "boomed/math/mat23f.h"
#include "boomed/world.h"
#include "boomed/generic/for_each.h"

DEF_FOR_EACH(vertex, vertex_t)
DEF_FOR_EACH(edge, edge_t)
DEF_FOR_EACH(zone, zone_t)


static void viewport_calc_transforms(viewport_t *viewport) {
    viewport->world_to_viewport = mat23f_mul(
        mat23f_mat22f_mul(
            mat23f_make_transform(vec2f_scalar_mul(viewport->size, 0.5f)),
            (mat22f_t){.x = {0.0f, -viewport->zoom}, .y = {viewport->zoom, 0.0f}}
        ),
        mat23f_make_transform(vec2f_negate(viewport->focus_world_pos))
    );

    viewport->viewport_to_world = mat23f_inverse(viewport->world_to_viewport);
}


void viewport_init(viewport_t *viewport) {
    viewport_calc_transforms(viewport);
}


void viewport_resize(viewport_t *viewport, vec2f_t size) {
    viewport->size = size;
    viewport_calc_transforms(viewport);
}


void viewport_start_pan(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_panning = true;
    viewport->pan_initial_viewport_pos = viewport_pos;
    viewport->pan_initial_world_pos = viewport->focus_world_pos;
}


void viewport_set_pan(viewport_t *viewport, vec2f_t viewport_pos) {
    assert(viewport->is_panning);
    vec2f_t delta = vec2f_sub(viewport_pos, viewport->pan_initial_viewport_pos);
    viewport->focus_world_pos = vec2f_sub(
        viewport->pan_initial_world_pos,
        mat22f_vec2f_mul(viewport->viewport_to_world.m, delta)
    );
    viewport_calc_transforms(viewport);
}


void viewport_stop_pan(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_panning = false;
}


void viewport_set_zoom(viewport_t *viewport, vec2f_t viewport_pos, float zoom_delta) {
    float old_zoom = viewport->zoom;
    viewport->zoom *= powf(1.25f, zoom_delta);
    vec2f_t world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, viewport_pos);
    viewport->focus_world_pos = vec2f_add(
        world_pos,
        vec2f_scalar_mul(vec2f_sub(viewport->focus_world_pos, world_pos), old_zoom / viewport->zoom)
    );
    viewport_calc_transforms(viewport);
}


static void draw_grid(const viewport_t *viewport) {
    vec2f_t top_left_world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero());
    vec2f_t grid_world_origin = vec2f_component_ceil(top_left_world_pos);
    vec2f_t grid_viewport_origin = mat23f_vec2f_mul(viewport->world_to_viewport, grid_world_origin);

    for (uint32_t n = 4; n < 9; n++) {
        int32_t gridw = 1 << n;

        float f = 1.0f - 0.25f * (n * n / 64.0f);
        uint32_t colour = ((uint32_t)(0x72 * f) << 0) | ((uint32_t)(0x8C * f) << 8) | ((uint32_t)(0x99 * f) << 16) | (0xFF << 24);

        // Plot vertical lines from left to right of the viewport
        // Note this corresponds to advancing in y world space
        int32_t startx = (int32_t)grid_world_origin.y;
        int32_t gridx = (startx + gridw - 1) & ~(gridw - 1);
        float x = grid_viewport_origin.x + (gridx - startx) * viewport->zoom;
        while (x < viewport->size.x) {
            if ((gridx & gridw) || n == 8) {
                draw_thick_line((vec2f_t){x, 0.0f}, (vec2f_t){x, viewport->size.y}, 1, colour);
            }
            gridx += gridw;
            x += gridw * viewport->zoom;
        }

        // Plot horizontal lines from top to bottom of the viewport
        // Note this corresponds to advancing in -x in world space
        int32_t starty = (int32_t)grid_world_origin.x;
        int32_t gridy = starty & ~(gridw - 1);
        float y = grid_viewport_origin.y + (starty - gridy) * viewport->zoom;
        while (y < viewport->size.y) {
            if ((gridy & gridw) || n == 8) {
                draw_thick_line((vec2f_t){0.0f, y}, (vec2f_t){viewport->size.x, y}, 1, colour);
            }
            gridy -= gridw;
            y += gridw * viewport->zoom;
        }
    }
}


static void draw_vertex(const void *ctx, uint32_t index, const vertex_t *vertex) {
    const viewport_t *viewport = ctx;

    vec2f_t world_pos = vec2f_make_from_vec2i(vertex->position);
    vec2f_t viewport_pos = mat23f_vec2f_mul(viewport->world_to_viewport, world_pos);

    draw_point(viewport_pos, 6, 0xFF300030);
}


static void draw_edge(const void *ctx, uint32_t index, const edge_t *edge) {
    const viewport_t *viewport = ctx;
    const world_t *world = viewport->world;
    const vertex_t *vertices = world->vertices.data;

    element_id_t start_vertex = edge->vertex_ids[0];
    element_id_t end_vertex = edge->vertex_ids[1];
    vec2f_t start_world_pos = vec2f_make_from_vec2i(vertices[start_vertex].position);
    vec2f_t end_world_pos = vec2f_make_from_vec2i(vertices[end_vertex].position);
    vec2f_t start_viewport_pos = mat23f_vec2f_mul(viewport->world_to_viewport, start_world_pos);
    vec2f_t end_viewport_pos = mat23f_vec2f_mul(viewport->world_to_viewport, end_world_pos);

    draw_thick_line(start_viewport_pos, end_viewport_pos, 3, 0xFF000000);
}


static void draw_zone(const void *ctx, uint32_t index, const zone_t *zone) {
    const viewport_t *viewport = ctx;
}


void viewport_render(const viewport_t *viewport) {
    const world_t *world = viewport->world;
    draw_grid(viewport);
    for_each_const_zone_ctx(world->zones.const_slice, draw_zone, viewport);
    for_each_const_edge_ctx(world->edges.const_slice, draw_edge, viewport);
    for_each_const_vertex_ctx(world->vertices.const_slice, draw_vertex, viewport);
}
