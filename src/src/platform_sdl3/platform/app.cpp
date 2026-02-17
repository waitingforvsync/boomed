#include "app.h"
#include <stdexcept>
#include <SDL3/SDL.h>


platform::app::app(const char* title, vec2<std::int32_t> size)
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error{SDL_GetError()};
    }

    SDL_Log("Video driver: %s", SDL_GetCurrentVideoDriver());

    if (!SDL_CreateWindowAndRenderer(title, size.x, size.y, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        throw std::runtime_error{SDL_GetError()};
    }

    if (!SDL_SetWindowMinimumSize(window, 200, 200)) {
        throw std::runtime_error{SDL_GetError()};
    }

    if (!SDL_SetRenderVSync(renderer, SDL_RENDERER_VSYNC_DISABLED)) {
        throw std::runtime_error{SDL_GetError()};
    }

    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        SDL_Log("Render driver %d: %s", i, SDL_GetRenderDriver(i));
    }
    SDL_Log("Chosen renderer: %s", SDL_GetRendererName(renderer));

    int ww = 0;
    int wh = 0;
    SDL_GetWindowSizeInPixels(window, &ww, &wh);
    SDL_Log("Window pixels: %d x %d", ww, wh);

    int rw = 0;
    int rh = 0;
    SDL_GetRenderOutputSize(renderer, &rw, &rh);
    SDL_Log("Render output: %d x %d", rw, rh);

    int vsync = 0;
    if (SDL_GetRenderVSync(renderer, &vsync)) {
        SDL_Log("Vsync: %d", vsync);
    }
}


platform::app::~app()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}


auto platform::app::size() const -> vec2<std::int32_t>
{
    auto result = vec2<std::int32_t>{};
    SDL_GetWindowSize(window, &result.x, &result.y);
    return result;
}


auto platform::app::clear(std::uint32_t color) const -> void
{
    SDL_SetRenderDrawColor(
        renderer,
        color & 0xFF,
        (color >> 8) & 0xFF,
        (color >> 16) & 0xFF,
        (color >> 24) & 0xFF
    );
    SDL_RenderClear(renderer);
}


auto platform::app::present() const -> void
{
    SDL_RenderPresent(renderer);
}
