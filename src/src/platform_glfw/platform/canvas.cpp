#include "canvas.h"
#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>


platform::canvas::canvas(const char* title, vec2<std::int32_t> size)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    win = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
    if (!win) {
        throw std::runtime_error{"Couldn't create window"};
    }

    glfwMakeContextCurrent(win);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
}


auto platform::canvas::size() const -> vec2<std::int32_t>
{    
    auto result = vec2<std::int32_t>{};
    glfwGetFramebufferSize(win, &result.x, &result.y);
    return result;
}


platform::canvas::~canvas()
{
    glfwDestroyWindow(win);
}


auto platform::canvas::clear(std::uint32_t color) const -> void
{
    glClearColor(
        (color & 0xFF) / 255.0f,
        ((color >> 8) & 0xFF) / 255.0f,
        ((color >> 16) & 0xFF) / 255.0f,
        ((color >> 24) & 0xFF) / 255.0f
    );
    glClear(GL_COLOR_BUFFER_BIT);
}


auto platform::canvas::present() const -> void
{
    glfwSwapBuffers(win);
}
