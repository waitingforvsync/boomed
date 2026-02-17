#ifndef BOOMED_PLATFORM_APP_H_
#define BOOMED_PLATFORM_APP_H_

#include "math/vec2.h"
struct GLFWwindow;


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
    GLFWwindow* window{};
};

} // namespace platform


#endif // ifndef BOOMED_PLATFORM_RENDERER_H_
