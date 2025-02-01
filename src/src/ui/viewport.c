#include "ui/viewport.h"
#include "platform_sdl3/main_sdl3.h"
#include "math/aabb2f.h"
#include "math/mat23f.h"
#include "app/boomed.h"
#include "world/subzone.h"
#include "world/zone.h"


static void viewport_calc_transforms(viewport_t *viewport) {
    viewport->world_to_viewport = mat23f_mul(
        mat23f_mat22f_mul(
            mat23f_make_transform(
                vec2f_scalar_mul(viewport->size, 0.5f)
            ),
            (mat22f_t) {
                vec2f_scalar_mul(vec2f_make_unity(), -viewport->zoom),
                vec2f_scalar_mul(vec2f_make_unitx(), viewport->zoom)
            }
        ),
        mat23f_make_transform(
            vec2f_negate(viewport->focus_world_pos)
        )
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
        mat22f_vec2f_mul(
            viewport->viewport_to_world.m,
            delta
        )
    );
    viewport_calc_transforms(viewport);
}


void viewport_pan_stop(viewport_t *viewport, vec2f_t viewport_pos) {
    viewport->is_panning = false;
}


typedef struct nearest_elements_t {
    element_id_t vertex_id;
    element_id_t edge_id;
} nearest_elements_t;

#define VERTEX_VIEWPORT_PIXEL_TOLERANCE (10.0f)
#define EDGE_VIEWPORT_PIXEL_TOLERANCE (6.0f)

static nearest_elements_t viewport_get_nearest_elements(const viewport_t *viewport, vec2f_t world_pos) {
    // Choose either the nearest vertex or the nearest edge, never both.
    // Vertices have a higher tolerance, as it's generally more useful to want to snap to a vertex than an edge.
    const world_t *world = &viewport->boomed->world;
    element_id_t vertex_id = world_find_vertex_closest_to_point(
        world,
        world_pos,
        VERTEX_VIEWPORT_PIXEL_TOLERANCE / viewport->zoom
    );

    element_id_t edge_id = ID_NONE;
    if (vertex_id == ID_NONE) {
        edge_id = world_find_edge_closest_to_point(
            world,
            world_pos,
            EDGE_VIEWPORT_PIXEL_TOLERANCE / viewport->zoom
        );
    }

    return (nearest_elements_t) {
        vertex_id,
        edge_id
    };
}


static vec2i_t vec2i_make_snapped(vec2f_t world_pos, int32_t snap) {
    return (vec2i_t) {
        lroundf(world_pos.x / snap) * snap,
        lroundf(world_pos.y / snap) * snap
    };
}


typedef struct point_info_t {
    vec2i_t world_pos;
    nearest_elements_t nearest;
} point_info_t;

static point_info_t viewport_get_point_info(const viewport_t *viewport, vec2f_t world_pos) {
    // 
    nearest_elements_t nearest = viewport_get_nearest_elements(viewport, world_pos);
    vec2i_t snapped_point = vec2i_make_snapped(world_pos, viewport->snap);

    return (point_info_t) {
        snapped_point,
        nearest
    };
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
    nearest_elements_t nearest = viewport_get_nearest_elements(viewport, world_pos);
    viewport->highlighted_vertex = nearest.vertex_id;
    viewport->highlighted_edge = nearest.edge_id;
}


void viewport_set_zoom(viewport_t *viewport, vec2f_t viewport_pos, float zoom_delta) {
    float old_zoom = viewport->zoom;
    viewport->zoom *= powf(1.25f, zoom_delta);
    vec2f_t world_pos = mat23f_vec2f_mul(viewport->viewport_to_world, viewport_pos);
    viewport->focus_world_pos = vec2f_add(
        world_pos,
        vec2f_scalar_mul(
            vec2f_sub(viewport->focus_world_pos, world_pos),
            old_zoom / viewport->zoom
        )
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

    static const uint32_t log2_max_grid_spacing = 10;

    for (uint32_t log2_grid_spacing = 3; log2_grid_spacing <= log2_max_grid_spacing; log2_grid_spacing++) {
        int32_t grid_spacing = 1 << log2_grid_spacing;

        float norm = log2_grid_spacing / (float)log2_max_grid_spacing;
        float f = norm * norm * 0.25f;
        uint32_t colour = ((uint32_t)(0xFF * f)) << 24;

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


static void viewport_draw_subzone(const viewport_t *viewport, const subzone_t *subzone, uint32_t colour, arena_t scratch) {
    const world_t *world = &viewport->boomed->world;
    vertices_view_t vertices = world->vertices.view;

    uint32_t num_vertices = subzone->vertex_ids.num;
    vec2fs_slice_t points = arena_new_slice(vec2f_t, num_vertices, &scratch);

    for (uint32_t n = 0; n < num_vertices; ++n) {
        vec2fs_slice_set(points, n,
            mat23f_vec2f_mul(
                viewport->world_to_viewport,
                vec2f_make_from_vec2i(
                    vertices_view_get(vertices, element_ids_get(&subzone->vertex_ids, n)).position
                )
            )
        );
    }
    draw_polygon(points.data, num_vertices, colour);
}

static void viewport_draw_zones(const viewport_t *viewport, arena_t scratch) {
    const world_t *world = &viewport->boomed->world;
    zones_view_t zones = world->zones.view;

    aabb2f_t viewport_aabb = aabb2f_make(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size)
    );

    static uint32_t zone_colours[16] = {
        0xFF808090, 0xFF809080, 0xFF908080, 0xFF809090, 0xFF908090, 0xFF909080, 0xFF708090, 0xFF708080,
        0xFF907080, 0xFF809070, 0xFF707080, 0xFF807080, 0xFF709080, 0xFF908070, 0xFF807070, 0xFF807090
    };

    for (uint32_t i = 0; i < world->zones.num; ++i) {
        const zone_t *zone = zones_view_get_ptr(zones, i);
        if (aabb2f_intersects(viewport_aabb, zone->aabb)) {
            for (uint32_t j = 0; j < zone->subzones.num; ++j) {
                viewport_draw_subzone(viewport, subzones_get_ptr(&zone->subzones, j), zone_colours[i & 0xF], scratch);
            }
        }
    }
}

static void viewport_draw_edges(const viewport_t *viewport) {
    const world_t *world = &viewport->boomed->world;
    vertices_view_t vertices = world->vertices.view;
    edges_view_t edges = world->edges.view;

    aabb2f_t viewport_aabb = aabb2f_make_with_margin(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size),
        VIEWPORT_EDGE_THICKNESS / viewport->zoom
    );

    for (uint32_t i = 0; i < edges.num; ++i) {
        if (aabb2f_intersects(viewport_aabb, edge_get_aabb(edges_view_get_ptr(edges, i), vertices))) {
            draw_thick_line(
                mat23f_vec2f_mul(viewport->world_to_viewport, vec2f_make_from_vec2i(vertices_view_get(vertices, edges_view_get(edges, i).vertex_ids[0]).position)),
                mat23f_vec2f_mul(viewport->world_to_viewport, vec2f_make_from_vec2i(vertices_view_get(vertices, edges_view_get(edges, i).vertex_ids[1]).position)),
                VIEWPORT_EDGE_THICKNESS,
                (i == viewport->highlighted_edge) ? 0xFF0000FF : 0xFF000000
            );
        }
    }
}

static void viewport_draw_vertices(const viewport_t *viewport) {
    const world_t *world = &viewport->boomed->world;
    vertices_view_t vertices = world->vertices.view;

    aabb2f_t viewport_aabb = aabb2f_make_with_margin(
        mat23f_vec2f_mul(viewport->viewport_to_world, vec2f_make_zero()),
        mat23f_vec2f_mul(viewport->viewport_to_world, viewport->size),
        VIEWPORT_VERTEX_RADIUS / viewport->zoom
    );

    for (uint32_t i = 0; i < vertices.num; ++i) {
        vec2f_t world_pos = vec2f_make_from_vec2i(vertices.data[i].position);
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
    viewport_draw_zones(viewport, viewport->boomed->scratch_arena);
    viewport_draw_grid(viewport);
    viewport_draw_edges(viewport);
    viewport_draw_vertices(viewport);
}
