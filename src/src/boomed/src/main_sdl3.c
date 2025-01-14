#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <cimgui/cimgui.h>
//#include <cimgui/backends/cimgui_impl_sdl3.h>
//#include <cimgui/backends/cimgui_impl_sdlrenderer3.h>
#include "main_sdl3.h"
#include "viewport.h"
#include "boomed/boomed.h"
#include "boomed/world.h"
#include "boomed/math/math.h"
#include "boomed/math/vec2f.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
//static bool show_demo_window = true;


static boomed_t boomed = {0};

static viewport_t viewport = {
	.boomed = &boomed,
	.highlighted_vertex = ID_NONE,
	.highlighted_edge = ID_NONE,
	.snap = 16,
	.zoom = 2.0f
};


static vec2f_t get_window_size(SDL_Window *win) {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	return (vec2f_t){(float)w, (float)h};
}


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	(void)appstate;
	(void)argc;
	(void)argv;
	
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	
	// Without this, window movement and general frame rate is choppy under the default direct3d12 renderer
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d");
	
	if (!SDL_CreateWindowAndRenderer("BoomEd", 1280, 960, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY, &window, &renderer)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	

	boomed_init(&boomed);

	//--- test code
    element_id_t v0 = world_add_vertex(&boomed.world, (vec2i_t) {0, 0}, &boomed.world_arena);
    element_id_t v1 = world_add_vertex(&boomed.world, (vec2i_t) {112, 48}, &boomed.world_arena);
    element_id_t v2 = world_add_vertex(&boomed.world, (vec2i_t) {0, 32}, &boomed.world_arena);
    element_id_t v3 = world_add_vertex(&boomed.world, (vec2i_t) {64, -32}, &boomed.world_arena);

    element_id_t e0 = world_add_edge(&boomed.world, v0, v2, 0, 0, &boomed.world_arena, boomed.scratch_arena);
    element_id_t e1 = world_add_edge(&boomed.world, v0, v3, 0, 0, &boomed.world_arena, boomed.scratch_arena);
    element_id_t e2 = world_add_edge(&boomed.world, v1, v2, 0, 0, &boomed.world_arena, boomed.scratch_arena);
    element_id_t e3 = world_add_edge(&boomed.world, v3, v1, 0, 0, &boomed.world_arena, boomed.scratch_arena);
    //--- end test code


	viewport.size = get_window_size(window);
	viewport_init(&viewport);

//	SDL_SetRenderVSync(renderer, 1);

#if 0
	// Setup Dear ImGui context
	// IMGUI_CHECKVERSION();
	igCreateContext(NULL);
	ImGuiIO* io = igGetIO();
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	igStyleColorsLight(NULL);

	// Setup Platform/Renderer backends
	igImplSDL3_InitForSDLRenderer(window, renderer);
	igImplSDLRenderer3_Init(renderer);
	
	ImFontAtlas_AddFontFromFileTTF(io->Fonts, "res/DroidSans.ttf", 14.0f, NULL, NULL);
#endif

	return SDL_APP_CONTINUE;
}
	

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
	(void)appstate;
	
#if 0
	igImplSDL3_ProcessEvent(event);
#endif
	
	switch (event->type) {

		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;

		case SDL_EVENT_WINDOW_RESIZED:
			viewport_resize(&viewport, (vec2f_t){(float)event->window.data1, (float)event->window.data2});
			break;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
			if (event->button.button == SDL_BUTTON_RIGHT) {
				viewport_pan_start(&viewport, (vec2f_t){event->button.x, event->button.y});
			}
			else if (event->button.button == SDL_BUTTON_LEFT) {
				viewport_action_start(&viewport, (vec2f_t){event->button.x, event->button.y});
			}
			break;

		case SDL_EVENT_MOUSE_BUTTON_UP:
			if (event->button.button == SDL_BUTTON_RIGHT) {
				viewport_pan_stop(&viewport, (vec2f_t){event->button.x, event->button.y});
			}
			else if (event->button.button == SDL_BUTTON_LEFT) {
				viewport_action_stop(&viewport, (vec2f_t){event->button.x, event->button.y});
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (event->motion.state & SDL_BUTTON_RMASK) {
				viewport_pan_move(&viewport, (vec2f_t){event->motion.x, event->motion.y});
			}
			else if (event->motion.state & SDL_BUTTON_LMASK) {
				viewport_action_move(&viewport, (vec2f_t){event->motion.x, event->motion.y});
			}
			viewport_update_mouse_pos(&viewport, (vec2f_t){event->motion.x, event->motion.y});
			break;
		
		case SDL_EVENT_MOUSE_WHEEL:
			viewport_set_zoom(&viewport, (vec2f_t){event->wheel.mouse_x, event->wheel.mouse_y}, event->wheel.y);
			break;

		case SDL_EVENT_KEY_DOWN:
			if (event->key.mod & SDL_KMOD_CTRL) {
				switch (event->key.key) {
					case SDLK_Z:
						viewport_command_undo(&viewport);
						break;
					case SDLK_Y:
						viewport_command_redo(&viewport);
						break;
					case SDLK_X:
						viewport_command_cut(&viewport);
						break;
					case SDLK_C:
						viewport_command_copy(&viewport);
						break;
					case SDLK_V:
						viewport_command_paste(&viewport);
						break;
				}
				break;
			}
	}
	
	return SDL_APP_CONTINUE;
}



SDL_AppResult SDL_AppIterate(void *appstate) {
	(void)appstate;

#if 0
	// Start the Dear ImGui frame
	igImplSDLRenderer3_NewFrame();
	igImplSDL3_NewFrame();
	igNewFrame();

	if (show_demo_window) {
		igShowDemoWindow(&show_demo_window);
	}
	
	// Rendering
	igRender();
	const ImGuiIO* io = igGetIO();
	SDL_SetRenderScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
#endif

	SDL_SetRenderDrawColor(renderer, 0x72, 0x8C, 0x99, 0xFF);
	SDL_RenderClear(renderer);
	
	viewport_render(&viewport);

#if 0
	igImplSDLRenderer3_RenderDrawData(igGetDrawData(), renderer);
#endif

	SDL_RenderPresent(renderer);
	
	return SDL_APP_CONTINUE;
}
	

void SDL_AppQuit(void *appstate, SDL_AppResult result) {	
	(void)appstate;
    (void)result;

#if 0	
	// Cleanup
	igImplSDLRenderer3_Shutdown();
	igImplSDL3_Shutdown();
	igDestroyContext(NULL);
#endif
}


static SDL_FColor fcolor_make_from_packed_rgba(uint32_t color) {
	return (SDL_FColor){
		(color & 0xFF) / 255.0f,
		((color >> 8) & 0xFF) / 255.0f,
		((color >> 16) & 0xFF) / 255.0f,
		((color >> 24) & 0xFF) / 255.0f
	};
}


void draw_thick_line(vec2f_t start, vec2f_t end, float width, uint32_t color) {
	bool has_alpha = ((color >> 24) < 0xFF);
	if (has_alpha) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}

	vec2f_t dir = vec2f_normalize(vec2f_sub(end, start));
	vec2f_t offset = vec2f_scalar_mul(vec2f_perp(dir), width * 0.5f);
	
	SDL_FColor vertex_color = fcolor_make_from_packed_rgba(color);
	
	vec2f_t vertex_pos[] = {
		vec2f_add(start, offset),
		vec2f_sub(start, offset),
		vec2f_add(end, offset),
		vec2f_sub(end, offset)
	};
	
	static uint8_t indices[] = {
		0, 1, 2,  2, 1, 3
	};
	
	SDL_RenderGeometryRaw(
		renderer,
		NULL,
		vec2f_as_floats(vertex_pos), sizeof *vertex_pos,
		&vertex_color, 0,
		NULL, 0,
		sizeof vertex_pos / (sizeof *vertex_pos),
		indices, sizeof indices / sizeof *indices, sizeof *indices
	);

	if (has_alpha) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}


#define NUM_POINT_VERTICES 10

void draw_point(vec2f_t centre, float radius, uint32_t color) {	
	vec2f_t vertex_pos[NUM_POINT_VERTICES];
	for (int i = 0; i < NUM_POINT_VERTICES; i++) {
		float angle = deg_to_rad(i * (360.0f / NUM_POINT_VERTICES));
		vertex_pos[i] = vec2f_add(centre, vec2f_scalar_mul(vec2f_make_cossin(angle), radius));
	}
	
	static uint8_t indices[(NUM_POINT_VERTICES - 2) * 3] = {
		0, 1, 2,  0, 2, 3,  0, 3, 4,  0, 4, 5,  0, 5, 6,  0, 6, 7,  0, 7, 8,  0, 8, 9
	};
	
	SDL_FColor vertex_color = fcolor_make_from_packed_rgba(color);

	SDL_RenderGeometryRaw(
		renderer,
		NULL,
		vec2f_as_floats(vertex_pos), sizeof *vertex_pos,
		&vertex_color, 0,
		NULL, 0,
		sizeof vertex_pos / (sizeof *vertex_pos),
		indices, sizeof indices / sizeof *indices, sizeof *indices
	);
}
