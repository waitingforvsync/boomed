#include "app.h"
#include <stdexcept>
#include <SDL3/SDL.h>


platform::app::app()
{
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        throw std::runtime_error{SDL_GetError()};
    }

    SDL_Log("Video driver: %s", SDL_GetCurrentVideoDriver());
}
