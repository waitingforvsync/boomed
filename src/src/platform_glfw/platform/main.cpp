#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "app/app.h"
#include <cstdio>


auto main(int argc, char* argv[]) -> int
{
    app boomed_app{"BoomEd", vec2<std::int32_t>{1280, 960}};
    boomed_app.run();
}
