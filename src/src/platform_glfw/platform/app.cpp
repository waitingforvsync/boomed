#include "app.h"
#include <stdexcept>
#include <glad/gl.h>
#include <GLFW/glfw3.h>


platform::app::app(const char* title, vec2<std::int32_t> size)
{
    if (!glfwInit()) {
        throw std::runtime_error{"Could not initialize GLFW"};
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);
    if (!window) {
        throw std::runtime_error{"Could not create GLFW window"};
    }

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    auto glfw_platform_name = [] {
        switch (glfwGetPlatform()) {
            case GLFW_PLATFORM_WIN32: return "Windows";
            case GLFW_PLATFORM_COCOA: return "Mac";
            case GLFW_PLATFORM_WAYLAND: return "Wayland";
            case GLFW_PLATFORM_X11: return "X11";
            case GLFW_PLATFORM_NULL: return "Null";
            default: return "Unknown";
        }
    };
    std::printf("Platform: %s\n", glfw_platform_name());

}


platform::app::~app()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


auto platform::app::run() -> void
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0x72 / 255.0f, 0x8C / 255.0f, 0x99 / 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }
}
