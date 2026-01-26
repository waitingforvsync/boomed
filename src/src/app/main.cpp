#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "math/vec2.h"

static SDL_Window* window;
static SDL_Renderer* renderer;

auto SDL_AppInit(void** app_state, int argc, char* argv[]) -> SDL_AppResult
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("BoomEd", 1280, 960, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer", SDL_GetError(), nullptr);
        return SDL_APP_FAILURE;
    }

    if (!SDL_SetWindowMinimumSize(window, 200, 200)) {
        SDL_Log("Couldn't set min size: %s", SDL_GetError());
    }

    if (!SDL_SetRenderVSync(renderer, 1)) {
        SDL_Log("VSync not supported: %s", SDL_GetError());
    }

    SDL_Log("Video driver: %s", SDL_GetCurrentVideoDriver());
    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        SDL_Log("Render driver %d: %s", i, SDL_GetRenderDriver(i));
    }
    SDL_Log("Chosen renderer: %s", SDL_GetRendererName(renderer));

    int ww = 0, wh = 0, rw = 0, rh = 0;
    SDL_GetWindowSizeInPixels(window, &ww, &wh);
    SDL_GetRenderOutputSize(renderer, &rw, &rh);
    SDL_Log("Window pixels: %d x %d", ww, wh);
    SDL_Log("Render output: %d x %d", rw, rh);

    int vsync = 0;
    if (SDL_GetRenderVSync(renderer, &vsync)) {
        SDL_Log("Vsync: %d", vsync);
    }

    return SDL_APP_CONTINUE;
}


auto SDL_AppEvent(void* app_state, SDL_Event* event) -> SDL_AppResult
{
    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}


auto SDL_AppIterate(void* app_state) -> SDL_AppResult
{
    SDL_SetRenderDrawColor(renderer, 0x72, 0x8C, 0x99, 0xFF);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}


auto SDL_AppQuit(void* app_state, SDL_AppResult result) -> void
{
}
