#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdexcept>
#include <cstring>
#include "app/app.h"
#ifdef WITH_TESTS
#include <catch2/catch_session.hpp>
#endif


auto SDL_AppInit(void** app_state, int argc, char* argv[]) -> SDL_AppResult
{
#ifdef WITH_TESTS
    if (argc == 2 && std::strcmp(argv[1], "--test") == 0) {
        auto result = Catch::Session().run(argc - 1, argv + 1);
        return (result == 0) ? SDL_APP_SUCCESS : SDL_APP_FAILURE;
    }
#endif

    try {
        *app_state = new app{"BoomEd", vec2<std::int32_t>{1280, 960}};
        return SDL_APP_CONTINUE;
    }
    catch (const std::runtime_error& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
        return SDL_APP_FAILURE;
    }
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
    app* a = static_cast<app*>(app_state);
    a->render();
    return SDL_APP_CONTINUE;
}


auto SDL_AppQuit(void* app_state, SDL_AppResult result) -> void
{
    app* a = static_cast<app*>(app_state); 
    delete a;
}
