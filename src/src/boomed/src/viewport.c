#include "viewport.h"
#include "main_sdl3.h"
#include "boomed/math/aabb2f.h"
#include "boomed/math/mat23f.h"
#include "boomed/boomed.h"


static void viewport_calc_transforms(viewport_t *viewport) {
    viewport->world_to_viewport = mat23f_mul(
        mat23f_mat22f_mul(
            mat23f_make_transform(vec2f_scalar_mul(viewport->size, 0.5f)),
            (mat22f_t){{0.0f, -viewport->zoom}, {viewport->zoom, 0.0f}}
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


void viewport_pan_start(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_panning = true;
    viewport->pan_initial_viewport_pos = viewport_pos;
    viewport->pan_initial_world_pos = viewport->focus_world_pos;
}


void viewport_pan_move(viewport_t *viewport, vec2f_t viewport_pos) {
    assert(viewport->is_panning);
    vec2f_t delta = vec2f_sub(viewport_pos, viewport->pan_initial_viewport_pos);
    viewport->focus_world_pos = vec2f_sub(
        viewport->pan_initial_world_pos,
        mat22f_vec2f_mul(viewport->viewport_to_world.m, delta)
    );
    viewport_calc_transforms(viewport);
}


void viewport_pan_stop(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_panning = false;
}


void viewport_action_start(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_dragging = true;
    viewport->action_initial_world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, viewport_pos);
}


void viewport_action_move(viewport_t *viewport, vec2f_t viewport_pos) {
    assert(viewport->is_dragging);
}


void viewport_action_stop(viewport_t *viewport, vec2f_t viewport_pos) {

}


void viewport_update_mouse_pos(viewport_t *viewport, vec2f_t viewport_pos) {
    vec2f_t world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, viewport_pos);
    const world_t *world = &viewport->boomed->world;

    viewport->highlighted_edge = ID_NONE;
    viewport->highlighted_vertex = vertex_find_closest_to_point(
        world->vertices,
        world->vertices_num,
        world_pos,
        10.0f / viewport->zoom
    );

    if (viewport->highlighted_vertex == ID_NONE) {
        viewport->highlighted_edge = edge_find_closest_to_point(
            world->edges,
            world->edges_num,
            world->vertices,
            world_pos,
            6.0f / viewport->zoom
        );
    }
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


void viewport_command_undo(viewport_t *viewport) {

}


void viewport_command_redo(viewport_t *viewport) {

}


void viewport_command_cut(viewport_t *viewport) {

}


void viewport_command_copy(viewport_t *viewport) {

}


void viewport_command_paste(viewport_t *viewport) {
    
}


static void viewport_draw_grid(const viewport_t *viewport) {
    vec2f_t top_left_world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero());
    vec2f_t grid_world_origin = vec2f_component_ceil(top_left_world_pos);
    vec2f_t grid_viewport_origin = mat23f_vec2f_mul(viewport->world_to_viewport, grid_world_origin);

    static const uint32_t log2_max_grid_spacing = 8;

    for (uint32_t log2_grid_spacing = 3; log2_grid_spacing <= log2_max_grid_spacing; log2_grid_spacing++) {
        int32_t grid_spacing = 1 << log2_grid_spacing;

        float norm = log2_grid_spacing / (float)log2_max_grid_spacing;
        float f = 1.0f - 0.375f * norm * norm;
        uint32_t colour = ((uint32_t)(0x72 * f) << 0) |
                          ((uint32_t)(0x8C * f) << 8) |
                          ((uint32_t)(0x99 * f) << 16) |
                          (0xFF << 24);

        // Plot vertical lines from left to right of the viewport
        // Note this corresponds to advancing in y world space
        int32_t startx = (int32_t)grid_world_origin.y;
        int32_t gridx = (startx + grid_spacing - 1) & ~(grid_spacing - 1);
        float x = grid_viewport_origin.x + (gridx - startx) * viewport->zoom;
        while (x < viewport->size.x) {
            if ((gridx & grid_spacing) || log2_grid_spacing == log2_max_grid_spacing) {
                draw_thick_line((vec2f_t){x, 0.0f}, (vec2f_t){x, viewport->size.y}, 1, colour);
            }
            gridx += grid_spacing;
            x += grid_spacing * viewport->zoom;
        }

        // Plot horizontal lines from top to bottom of the viewport
        // Note this corresponds to advancing in -x in world space
        int32_t starty = (int32_t)grid_world_origin.x;
        int32_t gridy = starty & ~(grid_spacing - 1);
        float y = grid_viewport_origin.y + (starty - gridy) * viewport->zoom;
        while (y < viewport->size.y) {
            if ((gridy & grid_spacing) || log2_grid_spacing == log2_max_grid_spacing) {
                draw_thick_line((vec2f_t){0.0f, y}, (vec2f_t){viewport->size.x, y}, 1, colour);
            }
            gridy -= grid_spacing;
            y += grid_spacing * viewport->zoom;
        }
    }
}


#define VIEWPORT_VERTEX_RADIUS 6
#define VIEWPORT_EDGE_THICKNESS 3


static void viewport_draw_zones(const viewport_t *viewport) {
    const world_t *world = &viewport->boomed->world;
    const vertex_t *vertices = world->vertices;
    const zone_t *zones = world->zones;

    aabb2f_t viewport_aabb = aabb2f_make(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size)
    );

    for (uint32_t i = 0; i < world->zones_num; ++i) {
    }
}

static void viewport_draw_edges(const viewport_t *viewport) {
    const world_t *world = &viewport->boomed->world;
    const vertex_t *vertices = world->vertices;
    const edge_t *edges = world->edges;

    aabb2f_t viewport_aabb = aabb2f_make_with_margin(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size),
        VIEWPORT_EDGE_THICKNESS / viewport->zoom
    );

    for (uint32_t i = 0; i < world->edges_num; ++i) {
        if (aabb2f_intersects(viewport_aabb, edge_get_aabb(edges + i, vertices))) {
            draw_thick_line(
                mat23f_vec2f_mul(viewport->world_to_viewport, vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[0]].position)),
                mat23f_vec2f_mul(viewport->world_to_viewport, vec2f_make_from_vec2i(vertices[edges[i].vertex_ids[1]].position)),
                VIEWPORT_EDGE_THICKNESS,
                (i == viewport->highlighted_edge) ? 0xFF0000FF : 0xFF000000
            );
        }
    }
}

static void viewport_draw_vertices(const viewport_t *viewport) {
    const world_t *world = &viewport->boomed->world;
    const vertex_t *vertices = world->vertices;

    aabb2f_t viewport_aabb = aabb2f_make_with_margin(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size),
        VIEWPORT_VERTEX_RADIUS / viewport->zoom
    );

    for (uint32_t i = 0; i < world->vertices_num; ++i) {
        vec2f_t world_pos = vec2f_make_from_vec2i(vertices[i].position);
        if (aabb2f_contains_point(viewport_aabb, world_pos)) {
            vec2f_t viewport_pos = mat23f_vec2f_mul(viewport->world_to_viewport, world_pos);
            draw_point(
                viewport_pos,
                VIEWPORT_VERTEX_RADIUS,
                (i == viewport->highlighted_vertex) ? 0xFF0000FF : 0xFF000000
            );
        }
    }
}

void viewport_render(const viewport_t *viewport) {
    viewport_draw_zones(viewport);
    viewport_draw_grid(viewport);
    viewport_draw_edges(viewport);
    viewport_draw_vertices(viewport);
}
