#ifndef BOOMED_PLATFORM_APP_H_
#define BOOMED_PLATFORM_APP_H_

#include "math/vec2.h"

struct SDL_Renderer;
struct SDL_Window;


namespace platform {

class app {
public:
    app(const char* title, vec2<std::int32_t> size);
    virtual ~app();

    auto run() -> void;
    auto size() const -> vec2<std::int32_t>;
    auto clear(std::uint32_t color) const -> void;
    auto present() const -> void;

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
};

} // namespace platform


#endif // ifndef BOOMED_PLATFORM_RENDERER_H_
