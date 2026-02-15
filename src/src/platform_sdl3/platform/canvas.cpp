#include "canvas.h"
#include <stdexcept>
#include <SDL3/SDL.h>


platform::canvas::canvas(const char* title, vec2<std::int32_t> size)
{
    if (!SDL_CreateWindowAndRenderer(title, size.x, size.y, SDL_WINDOW_RESIZABLE, &win, &rend)) {
        throw std::runtime_error{SDL_GetError()};
    }

    if (!SDL_SetWindowMinimumSize(win, 200, 200)) {
        throw std::runtime_error{SDL_GetError()};
    }

    if (!SDL_SetRenderVSync(rend, SDL_RENDERER_VSYNC_DISABLED)) {
        throw std::runtime_error{SDL_GetError()};
    }

    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        SDL_Log("Render driver %d: %s", i, SDL_GetRenderDriver(i));
    }
    SDL_Log("Chosen renderer: %s", SDL_GetRendererName(rend));

    int ww = 0;
    int wh = 0;
    SDL_GetWindowSizeInPixels(win, &ww, &wh);
    SDL_Log("Window pixels: %d x %d", ww, wh);

    int rw = 0;
    int rh = 0;
    SDL_GetRenderOutputSize(rend, &rw, &rh);
    SDL_Log("Render output: %d x %d", rw, rh);

    int vsync = 0;
    if (SDL_GetRenderVSync(rend, &vsync)) {
        SDL_Log("Vsync: %d", vsync);
    }
}


auto platform::canvas::size() const -> vec2<std::int32_t>
{
    auto result = vec2<std::int32_t>{};
    SDL_GetWindowSize(win, &result.x, &result.y);
    return result;
}


platform::canvas::~canvas()
{
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
}


auto platform::canvas::clear(std::uint32_t color) const -> void
{
    SDL_SetRenderDrawColor(
        rend,
        color & 0xFF,
        (color >> 8) & 0xFF,
        (color >> 16) & 0xFF,
        (color >> 24) & 0xFF
    );
    SDL_RenderClear(rend);
}


auto platform::canvas::present() const -> void
{
    SDL_RenderPresent(rend);
}
