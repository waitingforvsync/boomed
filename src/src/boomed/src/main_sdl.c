#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
//#include <cimgui/cimgui.h>
//#include <cimgui/backends/cimgui_impl_sdl3.h>
//#include <cimgui/backends/cimgui_impl_sdlrenderer3.h>
#include "boomed/math/math.h"
#include "boomed/math/vec2f.h"

static SDL_Window *window;
static SDL_Renderer *renderer;
//static bool show_demo_window = true;

void draw_line(vec2f_t start, vec2f_t end, float width, uint32_t color);
void draw_point(vec2f_t centre, float radius, uint32_t color);


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
	
	vec2f_t start = (vec2f_t){100.0f, 100.f};
	vec2f_t end = (vec2f_t){280.0f, 220.0f};
	draw_line(start, end, 3, 0xFF000000);
	draw_point(start, 4, 0xFFB0E0FF);
	draw_point(end, 4, 0xFFB0E0FF);

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


SDL_FColor fcolor_make_from_packed_rgba(uint32_t color) {
	return (SDL_FColor){
		(color & 0xFF) / 255.0f,
		((color >> 8) & 0xFF) / 255.0f,
		((color >> 16) & 0xFF) / 255.0f,
		((color >> 24) & 0xFF) / 255.0f
	};
}


void draw_line(vec2f_t start, vec2f_t end, float width, uint32_t color) {
	
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
}


void draw_point(vec2f_t centre, float radius, uint32_t color) {
	
	#define NUM_POINT_VERTICES 10
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
