#ifndef BOOMED_PLATFORM_DISPLAY_H_
#define BOOMED_PLATFORM_DISPLAY_H_

#include <cstdint>
#include "math/vec2.h"
struct SDL_Renderer;
struct SDL_Window;

namespace platform {

class display {
public:
    explicit display(const char* title, vec2<std::int32_t> size);
    virtual ~display();

    auto size() const -> vec2<std::int32_t>;

    auto clear(std::uint32_t color) const -> void;
    auto present() const -> void;

private:
    SDL_Window* win;
    SDL_Renderer* rend;
};

} // namespace platform


#endif // ifndef BOOMED_PLATFORM_DISPLAY_H_
